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
} // namespace sql

namespace Dixter
{
	namespace Database
	{
		class TQueryBuilder;
		
		class TValue;
		
		class TTable : public CopyConstructible
		{
		public:
			TTable(const TString& tableName, const std::list<TValue*>& valueList) noexcept;
			
			~TTable() noexcept;
			
			void createTable(sql::Statement* statement, bool dropIf = true);
			
			sql::PreparedStatement*
			insertValues(sql::Connection* connection);
			
			sql::PreparedStatement*
			updateValues(sql::Connection* connection);
			
			const TString& getTableName() const;
			
			const std::list<TValue*>& getValuesList() const;
			
			const TSize& getParametersSize() const;
		
		private:
			TSize m_paramsSize;
			
			TString m_tableName;
			
			std::list<TValue*> m_dbValueList;
			
			TQueryBuilder* m_queryBuilder;
		};
	} // namespace Database
} // namespace Dixter