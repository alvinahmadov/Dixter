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
				: m_databaseManager(manager)
		{ }
		
		const TDictionary::TSearchResult&
		TDictionary::search(TWord word, const TString& column, bool fullsearch) noexcept
		{
			auto __clause = fullsearch ? column + " LIKE \"" + word.data() + "%\""
									   : column + "=\"" + word.data() + "\"";
			
			if (not m_resultMap.empty())
				m_resultMap.clear();
			
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
		TDictionary::doSearch(TByte key, TDatabaseManager::TClause&& clause)
		{
			try
			{
				std::lock_guard<std::mutex> __lg(m_mutex);
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
			catch (sql::SQLException& e)
			{
				printerr(e.what())
			}
		}
		
		void TDictionary::fetch(const TString& table, TDatabaseManager::TClause& clause)
		{
			auto __cols = m_databaseManager->getColumns(table);
			auto __resultSetPtr = m_databaseManager->selectColumnsWhere(table, __cols, clause);
			
			while (__resultSetPtr->next())
			{
				for (TSize i { 0UL }; i < __cols.size(); ++i)
				{
					auto __colValue = __resultSetPtr->getString(i + 1);
					
					if (not __colValue.length())
						break;
					
					if (auto __pos = m_resultMap.find(__cols.at(i));
							__pos != m_resultMap.end())
						__pos->second.push_back(__colValue);
					else
						m_resultMap.emplace_hint(
								m_resultMap.cend(),
								__cols.at(i), std::vector<TString> { __colValue });
				}
			}
		}
	} // namespace OpenTranslate
} // namespace Dixter