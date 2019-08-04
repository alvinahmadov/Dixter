/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once


#include <map>
#include <vector>

#include "Value.hpp"


namespace Dixter
{
	namespace Database
	{
		/**
		 * @brief A wrapper for Database queries
		 * @namespace dix::dictionary
		 * */
		class QueryBuilder
		{
		public:
			/**
			 * @brief Initializes structures
			 * */
			QueryBuilder();
			
			~QueryBuilder();
			
			const string_t& describeQuery(const string_t& tableName);
			
			const string_t& dropQuery(const string_t& tableName);
			
			const string_t& createQuery(const string_t& tableName, const std::list<Value*>& dbValueList, size_t& parametersNum);
			
			const string_t& insertQuery(const string_t& tableName, const size_t& parametersNum);
			
			/**
			 * Create query
			 * */
			const string_t& selectQuery(const std::vector<string_t>& tables, const std::vector<string_t>& columns,
			                            ui32 indexColumn, ui32 leftTableIndex);
			
			const string_t& selectQuery(const string_t& table, const string_t& column, string_t clause = "");
			
			const string_t& selectQuery(const string_t& table, const std::vector<string_t>& columns, ui32 indexColumn);
			
			const string_t& selectQuery(const string_t& table, const std::vector<string_t>& columns, string_t clause);
			
			const string_t& selectLikeQuery(const std::vector<string_t>& tables, const std::vector<string_t>& columns,
			                                const string_t& text, ui32 comparatorColumn, ui32 leftTableIndex, ui32 fieldIndex,
			                                bool asRegex = false);
			
			const string_t& updateQuery(const string_t& tableName, const std::list<Value*>& dbValueList);
		
		private:
			/// Query string to build
			string_t m_query;
		};
	}
}