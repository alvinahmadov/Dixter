/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "Exception.hpp"
#include "Table.hpp"
#include "Value.hpp"
#include "QueryBuilder.hpp"

namespace Dixter
{
	namespace Database
	{
		TTable::TTable(const TString& tableName,
					   const std::list<TValue*>& valueList) noexcept
				: m_paramsSize(),
				  m_tableName(tableName),
				  m_dbValueList(valueList),
				  m_queryBuilder(new TQueryBuilder)
		{ }
		
		TTable::~TTable() noexcept
		{
			delete m_queryBuilder;
			
			for (auto& __i : m_dbValueList)
				delete __i;
		}
		
		void TTable::createTable(sql::Statement* statement, bool dropIf)
		{
			if (statement == nullptr)
				throw TIllegalArgumentException("%s:%d Statement is NULL.\n", __FILE__, __LINE__);
			
			TString __query;
			if (dropIf)
			{
				__query = m_queryBuilder->dropQuery(m_tableName);
				statement->execute(__query);
			}
			__query = m_queryBuilder->createQuery(m_tableName, m_dbValueList, m_paramsSize);
			if (__query.empty())
				throw TSQLException("%s:%d Query not set", __FILE__, __LINE__);
			
			statement->execute(__query);
		}
		
		sql::PreparedStatement*
		TTable::insertValues(sql::Connection* connection)
		{
			auto __query = m_queryBuilder->insertQuery(m_tableName, m_paramsSize);
			auto __result = connection->prepareStatement(__query.data());
			return __result;
		}
		
		sql::PreparedStatement*
		TTable::updateValues(sql::Connection* connection)
		{
			connection->getMetaData();
			return nullptr;
		}
		
		const TString&
		TTable::getTableName() const
		{
			return m_tableName;
		}
		
		const std::list<TValue*>&
		TTable::getValuesList() const
		{
			return m_dbValueList;
		}
		
		const TSize&
		TTable::getParametersSize() const
		{
			return m_paramsSize;
		}
	} // namespace Database
} // namespace Dixter