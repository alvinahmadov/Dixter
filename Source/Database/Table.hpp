/**
 *  File DBTable.h
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once


#include <map>
#include <list>
#include "Types.hpp"

namespace sql
{
	class Connection;
	
	class Statement;
	
	class PreparedStatement;
}

namespace Dixter
{
	namespace Database
	{
		class QueryBuilder;
		
		class Value;
		
		class Table
		{
		public:
			Table(const string_t& tableName, const std::list<Value*>& valueList);
			
			~Table();
			
			void createTable(sql::Statement* statement, bool dropIf = true);
			
			sql::PreparedStatement* insertValues(sql::Connection* connection);
			
			sql::PreparedStatement* updateValues(sql::Connection* connection);
			
			const string_t& getTableName() const;
			
			const std::list<Value*>& getValuesList() const;
			
			const size_t& getParametersSize() const;
		
		private:
			size_t m_paramsSize;
			
			string_t m_tableName;
			
			std::list<Value*>* m_dbValueList;
			
			QueryBuilder* m_queryBuilder;
		};
	}
}