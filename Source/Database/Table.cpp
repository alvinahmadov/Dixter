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

#include "Commons.hpp"
#include "Table.hpp"
#include "QueryBuilder.hpp"

using namespace std;

namespace Dixter
{
	namespace Database
	{
		Table::Table(const string_t& tableName, const list<Value*>& valueList)
				: m_paramsSize(),
				  m_tableName(tableName),
				  m_dbValueList(const_cast<list<Value*>*>(&valueList)),
				  m_queryBuilder(new QueryBuilder)
		{ }
		
		Table::~Table()
		{
			SAFE_RELEASE(m_queryBuilder)
			SAFE_RELEASE(m_dbValueList)
		}
		
		void Table::createTable(sql::Statement* statement, bool dropIf)
		{
			if (statement == nullptr)
			{
				throw IllegalArgumentException { "%s:%d Statement is NULL.\n", __FILE__, __LINE__ };
			}
			string_t query { };
			if (dropIf)
			{
				query = m_queryBuilder->dropQuery(m_tableName);
				statement->execute(query);
			}
			query = m_queryBuilder->createQuery(m_tableName, *m_dbValueList, m_paramsSize);
			if (query.empty())
			{
				throw IllegalArgumentException { "%s:%d Query not set", __FILE__, __LINE__ };
			}
			statement->execute(query);
		}
		
		sql::PreparedStatement*
		Table::insertValues(sql::Connection* connection)
		{
			auto query = m_queryBuilder->insertQuery(m_tableName, m_paramsSize);
			auto result = connection->prepareStatement(query);
			return result;
		}
		
		sql::PreparedStatement*
		Table::updateValues(sql::Connection* connection)
		{
			connection->getMetaData();
			return nullptr;
		}
		
		const string_t&
		Table::getTableName() const
		{
			return m_tableName;
		}
		
		const list<Value*>&
		Table::getValuesList() const
		{
			return *m_dbValueList;
		}
		
		const size_t&
		Table::getParametersSize() const
		{
			return m_paramsSize;
		}
	}
}