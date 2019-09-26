/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include <vector>
#include <mutex>
#include <list>

namespace Dixter
{
	namespace Database
	{
		class TValue;
		
		/**
		 * @brief A wrapper for Database queries
		 * @namespace dix::dictionary
		 * */
		class TQueryBuilder
		{
			using Self              = TQueryBuilder;
			using TStringVector     = std::vector<TString>;
			#ifdef HAVE_CXX17
			using TClause           = TStringView;
			using TQuery            = TStringView;
			#else
			using TClause           = TString;
			using TQuery            = TString;
			#endif
		public:
			/**
			 * @brief Initializes structures
			 * */
			TQueryBuilder() noexcept;
			
			~TQueryBuilder() noexcept = default;
			
			TQuery describeQuery(const TString& tableName);
			
			TQuery dropQuery(const TString& tableName);
			
			TQuery createQuery(const TString& tableName, const std::list<TValue*>& dbValueList,
							   TSize& parametersNum);
			
			TQuery insertQuery(const TString& tableName, const TSize& parametersNum);
			
			/**
			 * Create query
			 * */
			TQuery selectQuery(const TStringVector& tables, const TStringVector& columns,
							   UInt32 indexColumn, UInt32 leftTableIndex);
			
			TQuery selectQuery(const TString& table, const TString& column, TClause clause = "");
			
			TQuery selectQuery(const TString& table, const TStringVector& columns, UInt32 indexColumn);
			
			TQuery selectQuery(const TString& table, const TStringVector& columns, TClause clause);
			
			TQuery selectLikeQuery(const TStringVector& tables, const TStringVector& columns,
								   const TString& text, UInt32 comparatorColumn,
								   UInt32 leftTableIndex, UInt32 fieldIndex, bool asRegex = false);
			
			TQuery updateQuery(const TString& tableName, const std::list<TValue*>& dbValueList);
		
		private:
			TString& resetQuery(std::ostringstream& stream);
		
		private:
			TString m_query;
			
			mutable std::mutex m_mutex;
		};
	} // namespace Database
} // namespace Dixter