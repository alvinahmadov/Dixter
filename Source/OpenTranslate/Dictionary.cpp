/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include "Group.hpp"
#include "Macros.hpp"
#include "Constants.hpp"
#include "Dictionary.hpp"
#include "JoinThread.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		TDictionary::TDictionary(TDatabaseManagerPtr manager) noexcept
				: m_resultMap(),
				  m_databaseManager(manager)
		{ }
		
		TDictionary::TDictionary(TDictionary&& self) noexcept
				: m_resultMap(std::move(self.m_resultMap)),
				  m_databaseManager(std::move(self.m_databaseManager))
		{ }
		
		TDictionary&
		TDictionary::operator=(TDictionary&& self) noexcept
		{
			return Utilities::
			compareAssign(this, self, [&]() {
				m_databaseManager = std::move(self.m_databaseManager);
			});
		}
		
		const TDictionary::TSearchResult&
		TDictionary::lookFor(TWord word, const TString& keyColumn, bool fullsearch) noexcept
		{
			if (not m_resultMap.empty())
				m_resultMap.clear();
			
			auto __clause = fullsearch ? keyColumn + " LIKE \"" + word.data() + "%\""
									   : keyColumn + "=\"" + word.data() + "\"";
			try
			{
				const TByte __key = std::toupper(word[0]);
				JoinThread jthread(&TDictionary::doSearch, this, __key, __clause);
			}
			catch (const std::exception& e)
			{
				printerr(e.what())
			}
			
			return m_resultMap;
		}
		
		void
		TDictionary::doSearch(TByte key, TDatabaseManager::TClause clause)
		{
			try
			{
				auto __resultSetPtr = m_databaseManager->selectColumn(g_indexTable, g_indexColumn);
				while (__resultSetPtr->next())
				{
					if (auto __key = __resultSetPtr->getString(1)[0];
							__key == key)
					{
						const TString table = __resultSetPtr->getString(1);
						this->fetch(table, clause);
					}
				}
			}
			catch (sql::SQLException& e) { printerr(e.what()) }
		}
		
		void TDictionary::fetch(const TString& table, TDatabaseManager::TClause clause)
		{
			std::lock_guard<std::mutex> __lg(m_mutex);
			auto __cols = m_databaseManager->getColumns(table);
			auto __resultSetPtr = m_databaseManager->selectColumnsWhere(table, __cols, clause);
			
			while (__resultSetPtr->next())
			{
				for (TSize __i { 0UL }; __i < __cols.size(); ++__i)
				{
					auto&& __colValue = __resultSetPtr->getString(__i + 1);
					
					if (not __colValue.length())
						break;
					
					if (auto __pos = m_resultMap.find(__cols.at(__i)); __pos != m_resultMap.end())
						__pos->second.push_back(__colValue);
					else
						m_resultMap.emplace(__cols.at(__i), TSearchResult::mapped_type{ __colValue });
				}
			}
		}
		
	} // namespace OpenTranslate
} // namespace Dixter
