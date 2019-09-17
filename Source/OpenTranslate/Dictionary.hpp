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
		std::ostream& operator<<(std::ostream& out, const std::multimap<TString, std::vector<TString>>& rs);
		std::ostream& operator<<(std::ostream& out, const std::vector<TString>& sv);
		
		class TDictionary
		{
			using TDatabaseManager      = Database::TManager;
			using TDatabaseManagerPtr   = std::shared_ptr<TDatabaseManager>;
			using TStringVector         = std::vector<TString>;
			using TSearchResult         = std::multimap<TString, TStringVector>;
			#ifdef HAVE_CXX17
			using TWord = TStringView;
			#else
			using TWord = const TString&;
			#endif
		public:
			TDictionary(TDatabaseManagerPtr manager, TString table, TString column) noexcept;
			
			~TDictionary() noexcept = default;
			
			const TSearchResult&
			search(TWord word, const TString& column, bool asRegex = false) noexcept;
			
		protected:
			void _search(TByte key, TDatabaseManager::TClause clause);
			
			void _collect(const TString& table, TDatabaseManager::TClause clause);
		
		private:
			TString m_table;
			TString m_column;
			mutable std::mutex m_mutex;
			TSearchResult m_resultMap;
			TDatabaseManagerPtr m_databaseManager;
		};
	}
}