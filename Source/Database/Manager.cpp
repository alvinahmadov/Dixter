/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Database/QueryBuilder.hpp"
#include "Database/Manager.hpp"
#include "Database/Table.hpp"


using namespace std;
using namespace sql;

namespace Dixter
{
	namespace Database
	{
		Manager::Manager(const string_t& host, const string_t& database,
		                 const string_t& userName, const string_t& password,
		                 const string_t& port)
				: m_tables { new map<string_t, Table*> },
				  m_preparedStmts { new map<string_t, PreparedStatement*> },
				  m_queryBuilder { new QueryBuilder },
				  m_connectionManager(new ConnectionManager(host, database, userName, password, port))
		{
			try
			{
				m_driver = get_driver_instance();
				m_connection = m_driver->connect(m_connectionManager->_host, m_connectionManager->_name, m_connectionManager->_password);
				m_connection->setSchema(m_connectionManager->_database);
				m_statement = m_connection->createStatement();
			} catch (SQLException& e)
			{
				printerr(e.what())
			}
		}
		
		Manager::Manager(const ConnectionManager& cMan)
				: Manager(cMan._host, cMan._database, cMan._name, cMan._password)
		{ }
		
		Manager::~Manager()
		{
			for (auto& value : *m_tables)
			{
				PUTLF(value.first)
				PUTLF(value.second->getTableName())
				SAFE_RELEASE(value.second)
			}
			SAFE_RELEASE(m_tables)
			for (auto& value : *m_preparedStmts)
			{
				SAFE_RELEASE(value.second)
			}
			SAFE_RELEASE(m_preparedStmts)
			
			SAFE_RELEASE(m_statement)
			SAFE_RELEASE(m_connection)
			SAFE_RELEASE(m_connectionManager)
			SAFE_RELEASE(m_queryBuilder)
		}
		
		void Manager::createTable(const string_t& tableName, const list<Value*>& valueList, bool drop)
		{
			m_tables->emplace(tableName, new Table(tableName, valueList));
			(*m_tables)[tableName]->createTable(m_statement, drop);
		}
		
		void Manager::insertValues(const string_t& tableName)
		{
			if (m_tables->find(tableName) != m_tables->end())
			{
				m_preparedStmts->emplace(tableName, (*m_tables)[tableName]->insertValues(m_connection));
			}
		}
		
		ResultSet*
		Manager::selectColumn(const string_t& table, const string_t& column, string_t clause)
		{
			string_t __query { m_queryBuilder->selectQuery(table, column, clause) };
			auto result = m_statement->executeQuery(__query);
			return result;
		}
		
		vector<string_t>
		Manager::getColumns(const string_t& table)
		{
			string_t query { m_queryBuilder->describeQuery(table) };
			vector<string_t> columns { };
			auto result = m_statement->executeQuery(query);
			
			while (result->next())
			{
				columns.push_back(result->getString(1));
			}
			return columns;
		}
		
		ResultSet*
		Manager::selectColumns(const vector<string_t>& tables, const vector<string_t>& columns,
		                       ui32 comparatorColumn, ui32 leftTableIndex)
		{
			string_t query { m_queryBuilder->selectQuery(tables, columns, comparatorColumn, leftTableIndex) };
			auto result = m_statement->executeQuery(query);
			return result;
		}
		
		ResultSet*
		Manager::selectColumnsWhere(const string_t& table, const vector<string_t>& columns, string_t clause)
		{
			auto result = m_statement->executeQuery(m_queryBuilder->selectQuery(table, columns, "WHERE " + clause));
			return result;
		}
		
		ResultSet*
		Manager::selectColumns(const string_t& table, const vector<string_t>& columns, ui32 comparatorColumn)
		{
			string_t query { m_queryBuilder->selectQuery(table, columns, comparatorColumn) };
			auto result = m_statement->executeQuery(query);
			return result;
		}
		
		ResultSet*
		Manager::selectColumnsLike(const vector<string_t>& tables, const vector<string_t>& columns,
		                           const string_t& searchText, ui32 comparatorColumn, ui32 leftTableIndex,
		                           ui32 fieldIndex)
		{
			string_t query { m_queryBuilder->selectLikeQuery(tables, columns, searchText, comparatorColumn, leftTableIndex, fieldIndex) };
			auto result = m_statement->executeQuery(query);
			return result;
		}
		
		void Manager::setBigInt(const string_t& tableName, ui32 parameterIndex, const string_t& value)
		{
			(*m_preparedStmts)[tableName]->setBigInt(parameterIndex, value);
		}
		
		void Manager::setBlob(const string_t& tableName,
		                      ui32 parameterIndex,
		                      istream* blob)
		{
			(*m_preparedStmts)[tableName]->setBlob(parameterIndex, blob);
		}
		
		void Manager::setBoolean(const string_t& tableName,
		                         ui32 parameterIndex,
		                         bool value)
		{
			(*m_preparedStmts)[tableName]->setBoolean(parameterIndex, value);
		}
		
		void Manager::setDateTime(const string_t& tableName,
		                          ui32 parameterIndex,
		                          const string_t& value)
		{
			(*m_preparedStmts)[tableName]->setDateTime(parameterIndex, SQLString { value.c_str() });
		}
		
		void Manager::setDouble(const string_t& tableName,
		                        ui32 parameterIndex,
		                        d32 value)
		{
			(*m_preparedStmts)[tableName]->setDouble(parameterIndex, value);
		}
		
		void Manager::setInt(const string_t& tableName,
		                     ui32 parameterIndex,
		                     i32 value)
		{
			(*m_preparedStmts)[tableName]->setInt(parameterIndex, value);
		}
		
		void Manager::setUInt(const string_t& tableName,
		                      ui32 parameterIndex,
		                      ui32 value)
		{
			(*m_preparedStmts)[tableName]->setUInt(parameterIndex, value);
		}
		
		void Manager::setInt64(const string_t& tableName,
		                       ui32 parameterIndex,
		                       i64 value)
		{
			(*m_preparedStmts)[tableName]->setInt64(parameterIndex, value);
		}
		
		void Manager::setUInt64(const string_t& tableName,
		                        ui32 parameterIndex,
		                        ui64 value)
		{
			(*m_preparedStmts)[tableName]->setUInt64(parameterIndex, value);
		}
		
		void Manager::setNull(const string_t& tableName,
		                      ui32 parameterIndex)
		{
			auto valList = (*m_tables)[tableName]->getValuesList();
			uint counter { 0 };
			DataType dataType = DataType::kUnknown;
			for (const auto& v : valList)
			{
				if (counter == parameterIndex)
				{
					dataType = v->getType();
				}
				++counter;
			}
			(*m_preparedStmts)[tableName]->setNull(parameterIndex, static_cast<int>(dataType));
		}
		
		void Manager::setString(const string_t& tableName,
		                        ui32 parameterIndex,
		                        const string_t& value)
		{
			(*m_preparedStmts)[tableName]->setString(parameterIndex, value);
		}
		
		ui32 Manager::setStrings(const string_t& tableName,
		                         const ui32& parameterStartIndex,
		                         list<string_t>& list,
		                         ui32 stringColumnNum)
		{
			if (stringColumnNum == 0)
			{
				stringColumnNum = static_cast<ui32>(list.size());
			}
			const ui32 __limit = stringColumnNum + parameterStartIndex;
			const string_t empty { };
			ui32 index { parameterStartIndex };
			if ((*m_preparedStmts)[tableName] == nullptr)
			{
				throw Exception { "No statement is prepared" };
			}
			try
			{
				for (; index < __limit; ++index)
				{
					if (list.size() != 0)
					{
						setString(tableName, index, list.back());
						list.pop_back();
					} else
					{
						setString(tableName, index, empty);
					}
				}
			} catch (exception& e)
			{
				printerr(e.what())
			}
			list.clear();
			return index;
		};
		
		int Manager::executeUpdate(const string_t& tableName)
		{
			auto result = (*m_preparedStmts)[tableName]->executeUpdate();
			return result;
		}
		
		ResultSet*
		Manager::execute(const string_t& tableName, string_t query)
		{
			return (*m_preparedStmts)[tableName]->executeQuery(query);
		}
	}
}
