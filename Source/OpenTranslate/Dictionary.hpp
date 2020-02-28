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
#include <deque>

#include "setup.h"
#include "Database/Manager.hpp"

namespace Dixter
{
	template<
			typename T,
			typename ID
	>
	class TGroup;
	
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
		
		class TDictionary : public TMovable
		{
			using TDatabaseManager      = Database::TManager;
			using TDatabaseManagerPtr   = std::shared_ptr<TDatabaseManager>;
			#ifdef HAVE_CXX17
			using TWord = TStringView;
			#else
			using TWord = const TString&;
			#endif
		
		public:
			using TSearchResult = std::unordered_multimap<TString, std::vector<TString>>;
		
		public:
			explicit TDictionary(TDatabaseManagerPtr manager) noexcept;
			
			TDictionary(TDictionary&& self) noexcept;
			
			TDictionary& operator=(TDictionary&& rv) noexcept;
			
			~TDictionary() noexcept = default;
			
			const TSearchResult&
			lookFor(TWord word, const TString& keyColumn, bool fullsearch = false) noexcept;
		
		protected:
			void doSearch(TByte key, TDatabaseManager::TClause clause);
			
			void fetch(const TString& table, TDatabaseManager::TClause clause);
		
		private:
			TSearchResult m_resultMap;
			
			TDatabaseManagerPtr m_databaseManager;
			
			mutable std::mutex m_mutex;
		};
	} // namespace OpenTranslate
} // namespace Dixter
