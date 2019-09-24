/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <mutex>

#include "Group.hpp"
#include "Database/Manager.hpp"
#include "setup.h"

namespace Dixter
{
	namespace OpenTranslate
	{
		inline TString toString(std::unordered_multimap<TString, std::vector<TString>>& resultMap)
		{
			std::ostringstream __oss{};
			
			for(const auto&[__k,__vs] : resultMap)
			{
				__oss << __k << ": {";
				for (const auto& __v : __vs)
					__oss << __v << " ";
				
				__oss << "}\n";
			}
			
			return __oss.str();
		}
		
		class TDictionary
		{
			using TDatabaseManager      = Database::TManager;
			using TDatabaseManagerPtr   = std::shared_ptr<TDatabaseManager>;
			using TStringVector         = std::vector<TString>;
			using TSearchResult         = std::unordered_multimap<TString, TStringVector>;
			#ifdef HAVE_CXX17
			using TWord = TStringView;
			#else
			using TSentence = const TString&;
			#endif
		public:
			TDictionary(TDatabaseManagerPtr manager, TString table, TString column) noexcept;
			
			~TDictionary() noexcept = default;
			
			const TSearchResult&
			search(TWord word, const TString& column, bool fullsearch = false) noexcept;
			
		protected:
			void doSearch(TByte key, TDatabaseManager::TClause clause);
			
			void fetch(const TString& table, TDatabaseManager::TClause clause);
		
		private:
			TString m_table;
			TString m_column;
			mutable std::mutex m_mutex;
			TSearchResult m_resultMap;
			TDatabaseManagerPtr m_databaseManager;
		};
	}
}