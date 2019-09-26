/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "Exception.hpp"
#include "Constants.hpp"
#include "Configuration.hpp"
#include "Database/QueryBuilder.hpp"
#include "Database/Manager.hpp"
#include "Database/Table.hpp"
#include "Database/Value.hpp"

namespace Dixter
{
	namespace Database
	{
		using TLockGuard = std::lock_guard<std::mutex>;
		
		// TConnectionManager implementation
		TManager::
		TConnectionManager::TConnectionManager(const TString& hostName,
											   const TString& userName,
											   const TString& password) noexcept
				: name_(userName),
				  password_(password)
		{
			TString __dbPort{};
			try
			{
				__dbPort = getIniManager({ g_guiConfigPath })
						->accessor()
						->getValue(NodeKey::kDatabasePortNode).asUTF8();
			}
			catch (TException& e)
			{
				printerr(e.getMessage())
			}
			host_ = Utilities::Strings::concat(g_dbProtocol, hostName, ":", __dbPort);
		}
		
		// TManager implementation
		TManager::TManager(const TString& hostName, const TString& dbUser, const TString& dbPassword) noexcept
				: m_driver(nullptr),
				  m_connection(nullptr),
				  m_statement(nullptr),
				  m_tables(),
				  m_prepStmts(),
				  m_queryBuilder(new TQueryBuilder),
				  m_connManager(hostName, dbUser, dbPassword)
		{
			try
			{
				m_driver = get_driver_instance();
				m_connection = m_driver->connect(
						m_connManager.host_,
						m_connManager.name_,
						m_connManager.password_);
				m_statement = m_connection->createStatement();
			}
			catch (sql::SQLException& e)
			{
				printerr(e.what())
			}
		}
		
		TManager::TManager(const TConnectionManager& cMan) noexcept
				: TManager(cMan.host_, cMan.name_, cMan.password_)
		{ }
		
		TManager::~TManager() noexcept
		{
			try
			{
				for (auto& value : m_tables)
					delete value.second;
				
				for (auto& value : m_prepStmts)
					delete value.second;
				
				delete m_statement;
				
				if (not m_connection->isClosed())
					m_connection->close();
				
				delete m_connection;
				
				delete m_queryBuilder;
			}
			catch (std::exception& e)
			{
				printerr(e.what())
			}
		}
		
		void TManager::selectDatabase(const TString& database)
		{
			m_connection->setSchema(database);
		}
		
		void TManager::createTable(const TString& tableName,
								   const std::list<TValue*>& valueList, bool drop)
		{
			TLockGuard __lockGuard(m_mutex);
			m_tables.emplace(tableName, new TTable(tableName, valueList));
			m_tables.at(tableName)->createTable(m_statement, drop);
		}
		
		void TManager::insertValues(const TString& tableName)
		{
			TLockGuard __lockGuard(m_mutex);
			if (m_tables.find(tableName) != m_tables.end())
				m_prepStmts.emplace(
						tableName,
						m_tables.at(tableName)->insertValues(m_connection));
		}
		
		std::vector<TString>
		TManager::getColumns(const TString& table)
		{
			TString __query(m_queryBuilder->describeQuery(table));
			std::vector<TString> __columns {};
			
			TLockGuard __lockGuard(m_mutex);
			auto __result = m_statement->executeQuery(__query);
			
			while (__result->next())
				__columns.push_back(__result->getString(1));
			
			return __columns;
		}
		
		TManager::TResultSetPtr
		TManager::selectColumn(const TString& table,
							   const TString& column, TClause clause)
		{
			TLockGuard __lockGuard(m_mutex);
			TString __query(m_queryBuilder->selectQuery(table, column, clause));
			return this->execute(__query);
		}
		
		TManager::TResultSetPtr
		TManager::selectColumns(const std::vector<TString>& tables,
								const std::vector<TString>& columns,
								UInt32 comparatorColumn, UInt32 leftTableIndex)
		{
			TLockGuard __lockGuard(m_mutex);
			TString __query(m_queryBuilder->selectQuery(tables, columns, comparatorColumn, leftTableIndex));
			return this->execute(__query);
		}
		
		TManager::TResultSetPtr
		TManager::selectColumnsWhere(const TString& table,
									 const std::vector<TString>& columns,
									 TClause clause)
		{
			TLockGuard __lockGuard(m_mutex);
			auto __clause = TString("WHERE ") + clause.data();
			TString __query(m_queryBuilder->selectQuery(table, columns, __clause));
			return this->execute(__query);
		}
		
		TManager::TResultSetPtr
		TManager::selectColumns(const TString& table,
								const std::vector<TString>& columns,
								UInt32 comparatorColumn)
		{
			TLockGuard __lockGuard(m_mutex);
			TString __query(m_queryBuilder->selectQuery(table, columns, comparatorColumn));
			return this->execute(__query);
		}
		
		TManager::TResultSetPtr
		TManager::selectColumnsLike(const std::vector<TString>& tables,
									const std::vector<TString>& columns,
									const TString& searchText,
									UInt32 comparatorColumn, UInt32 leftTableIndex,
									UInt32 fieldIndex)
		{
			TLockGuard __lockGuard(m_mutex);
			TString __query(
					m_queryBuilder->selectLikeQuery(
							tables, columns,
							searchText, comparatorColumn,
							leftTableIndex, fieldIndex));
			return this->execute(__query);
		}
		
		void TManager::setBigInt(const TString& tableName, UInt32 parameterIndex, const TString& value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setBigInt(parameterIndex, value);
		}
		
		void TManager::setBlob(const TString& tableName, UInt32 parameterIndex, std::istream* blob)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setBlob(parameterIndex, blob);
		}
		
		void TManager::setBoolean(const TString& tableName, UInt32 parameterIndex, bool value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setBoolean(parameterIndex, value);
		}
		
		void TManager::setDateTime(const TString& tableName, UInt32 parameterIndex, const TString& value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setDateTime(parameterIndex, sql::SQLString { value.data() });
		}
		
		void TManager::setDouble(const TString& tableName, UInt32 parameterIndex, Real32 value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setDouble(parameterIndex, value);
		}
		
		void TManager::setInt(const TString& tableName, UInt32 parameterIndex, Int32 value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setInt(parameterIndex, value);
		}
		
		void TManager::setUInt(const TString& tableName, UInt32 parameterIndex, UInt32 value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setUInt(parameterIndex, value);
		}
		
		void TManager::setInt64(const TString& tableName, UInt32 parameterIndex, Int64 value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setInt64(parameterIndex, value);
		}
		
		void TManager::setUInt64(const TString& tableName, UInt32 parameterIndex, UInt64 value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setUInt64(parameterIndex, value);
		}
		
		void TManager::setNull(const TString& tableName, UInt32 parameterIndex)
		{
			TLockGuard __lockGuard(m_mutex);
			auto __valueList = m_tables.at(tableName)->getValuesList();
			uint __counter { 0U };
			EDataType __dataType = EDataType::kUnknown;
			for (const auto& __value : __valueList)
			{
				if (__counter == parameterIndex)
					__dataType = __value->getType();
				++__counter;
			}
			m_prepStmts.at(tableName)->setNull(parameterIndex, static_cast<int>(__dataType));
		}
		
		void TManager::setString(const TString& tableName, UInt32 parameterIndex, const TString& value)
		{
			TLockGuard __lockGuard(m_mutex);
			m_prepStmts.at(tableName)->setString(parameterIndex, value);
		}
		
		UInt32 TManager::setStrings(const TString& tableName,
									const UInt32& parameterStartIndex,
									std::list<TString>& list,
									UInt32 stringColumnNum)
		{
			TLockGuard __lockGuard(m_mutex);
			if (not stringColumnNum)
				stringColumnNum = static_cast<UInt32>(list.size());
			
			const UInt32 __limit(stringColumnNum + parameterStartIndex);
			const TString __empty {};
			UInt32 __index(parameterStartIndex);
			if (not m_prepStmts.at(tableName))
				throw TSQLException("%s:%d No statement is prepared", __FILE__, __LINE__);
			
			try
			{
				for (; __index < __limit; ++__index)
				{
					if (list.size() != 0)
					{
						this->setString(tableName, __index, list.back());
						list.pop_back();
					}
					else
						this->setString(tableName, __index, __empty);
				}
			}
			catch (std::exception& e)
			{
				printerr(e.what())
			}
			list.clear();
			return __index;
		}
		
		int TManager::executeUpdate(const TString& tableName)
		{
			TLockGuard __lockGuard(m_mutex);
			int __result = m_prepStmts.at(tableName)->executeUpdate();
			return __result;
		}
		
		TManager::TResultSetPtr
		TManager::execute(const TString& query)
		{
			return TResultSetPtr(m_statement->executeQuery(query));
		}
	} // namespace Database
} // namespace Dixter