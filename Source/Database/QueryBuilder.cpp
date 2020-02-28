/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
 
#include "Constants.hpp"
#include "QueryBuilder.hpp"
#include "Utilities.hpp"
#include "Value.hpp"

namespace Dixter
{
	namespace Database
	{
		namespace AlgoUtils	= Utilities::Algorithms;
		using TLockGuard	= std::lock_guard<std::mutex>;
		using TQueryStream	= std::ostringstream;
		
		TQueryBuilder::TQueryBuilder() noexcept
				: m_query()
		{ }
		
		TQueryBuilder::TQuery
		TQueryBuilder::describeQuery(const TString& tableName)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream {};
			
			__queryStream << "DESCRIBE " << tableName;
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::dropQuery(const TString& tableName)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream {};
			
			__queryStream << "DROP TABLE IF EXISTS " << tableName;
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::createQuery(const TString& tableName,
								   const std::list<TValue*>& dbValueList,
								   TSize& parametersNum)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream {};
			
			__queryStream << "CREATE TABLE " << tableName << " (";
			
			for (const auto& __value : dbValueList)
			{
				bool __isAutoIncr(__value->isAutoIncrement());
				bool __isNull(__value->isNull());
				bool __isPrimaryKey { __value->isPrimaryKey() };
				
				__queryStream << " " << __value->getValueName() << " " << __value->getTypeString();
				
				if (__value->getSize() != 0)
					__queryStream << " (" << std::to_string(__value->getSize()) << ") ";
				
				if (__isAutoIncr)
					__queryStream << " AUTO_INCREMENT";
				
				if (__isNull)
					__queryStream << " NULL";
				else
					__queryStream << " NOT NULL";
				
				if (__isPrimaryKey)
					__queryStream << ", PRIMARY KEY (" << __value->getValueName() << ")";
				
				if (++parametersNum != dbValueList.size())
					__queryStream << ',';
			}
			__queryStream << ')';
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::insertQuery(const TString& tableName, const TSize& parametersNum)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream {};
			
			TSize __index {};
			__queryStream << "INSERT INTO " << tableName << " VALUES (";
			AlgoUtils::forEach(
					__index, parametersNum,
					[ & ](TSize&)
					{
						__queryStream << '?';
						if (__index != parametersNum - 1)
							__queryStream << ", ";
					});
			__queryStream << ')';
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::selectQuery(const TStringVector& tables, const TStringVector& columns,
								   UInt32 indexColumn, UInt32 leftTableIndex)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream {};
			
			__queryStream << "SELECT ";
			
			for (UInt32 __index = 0; __index < columns.size(); ++__index)
			{
				if (__index != indexColumn)
					__queryStream << columns.at(__index);
				if (__index != columns.size() - 1 and __index != indexColumn)
					__queryStream << ',';
				__queryStream << g_whiteSpace;
			}
			
			__queryStream << " FROM " << tables.at(leftTableIndex);
			if (tables.size() > 1)
			{
				__queryStream << " JOIN (";
				for (UInt32 __index = 0; __index < tables.size(); ++__index)
				{
					if (__index != leftTableIndex)
						__queryStream << tables.at(__index);
					
					if (__index != tables.size() - 1 and __index != leftTableIndex)
						__queryStream << ", ";
				}
				__queryStream << ") ON ";
			}
			
			for (UInt32 __i = 0; __i < tables.size(); ++__i)
			{
				if (tables.at(leftTableIndex) != tables.at(__i) and leftTableIndex != __i)
				{
					__queryStream << tables.at(leftTableIndex) << '.'
								  << columns.at(indexColumn) << '='
								  << tables.at(__i) << '.'
								  << columns.at(indexColumn);
					if (__i != tables.size() - 1)
						__queryStream << " AND ";
				}
			}
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::selectQuery(const TString& table, const TString& column, TClause clause)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream;
			
			__queryStream << "SELECT " << column << " FROM " << table;
			
			if (not clause.empty())
				__queryStream << " WHERE " << clause;
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::selectQuery(const TString& table, const TStringVector& columns, UInt32 indexColumn)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream;
			
			__queryStream << "SELECT ";
			
			for (UInt32 index = 0; index < columns.size(); index++)
			{
				if (index != indexColumn)
					__queryStream << columns.at(index);
				if (index != columns.size() - 1 and index != indexColumn)
					__queryStream << ',';
				__queryStream << g_whiteSpace;
			}
			
			__queryStream << " FROM " << table;
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::selectQuery(const TString& table, const TStringVector& columns, TClause clause)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream;
			uint index {};
			
			__queryStream << "SELECT ";
			
			for (const auto& column : columns)
				__queryStream << column << ( index++ == columns.size() - 1 ? "" : ", " );
			
			__queryStream << g_whiteSpace << " FROM " << table << g_whiteSpace << clause;
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::selectLikeQuery(const TStringVector& tables, const TStringVector& columns,
									   const TString& text, UInt32 comparatorColumn,
									   UInt32 leftTableIndex, UInt32 fieldIndex, bool asRegex)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream;
			
			this->selectQuery(tables, columns, comparatorColumn, leftTableIndex);
			__queryStream << " WHERE " << columns.at(fieldIndex)
						  << " LIKE \"" << ( asRegex ? "\'%" : "" )
						  << text << '%' << "\"";
			
			return this->resetQuery(__queryStream);
		}
		
		TQueryBuilder::TQuery
		TQueryBuilder::updateQuery(const TString& tableName, const std::list<TValue*>& dbValueList)
		{
			TLockGuard __lockGuard(m_mutex);
			TQueryStream __queryStream;
			
			for (const auto& __value : dbValueList)
				__queryStream << "UPDATE " << tableName
							  << " SET " << __value->getValueName() << "=, ";
			
			return this->resetQuery(__queryStream);
		}
		
		inline TString&
		TQueryBuilder::resetQuery(std::ostringstream& stream)
		{
			m_query.clear();
			m_query = stream.str();
			return m_query;
		}
	} // namespace Database
} // namespace Dixter