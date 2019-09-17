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
#include "Dictionary.hpp"
#include "JoinThread.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		std::ostream& operator<<(std::ostream& out, const std::vector<TString>& sv)
		{
			if(sv.empty())
				return out;
			out << '[';
			uint index {};
			for (const auto& v : sv)
				out << v << (index++ != sv.size() - 1 ? ", " : "]");
			
			return out;
		}
		
		std::ostream& operator<<(std::ostream& out, const std::multimap<TString, std::vector<TString>>& rs)
		{
			if(rs.empty())
				return out;
			out << '{';
			
			uint oidx {};
			for(const auto&[__k, __sv] : rs)
				out << __k << ": " << __sv << (oidx++ != rs.size() - 1 ? ",\n" : "}" );
			
			return out;
		}
		
		TDictionary::TDictionary(TDatabaseManagerPtr manager, TString table, TString column) noexcept
				: m_table { table },
				  m_column { column },
				  m_databaseManager { manager }
		{ }
		
		const TDictionary::TSearchResult&
		TDictionary::search(TWord word, const TString& column, bool asRegex) noexcept
		{
			auto __clause = asRegex ? column + " LIKE \"" + word.data() + "%\""
			                        : column + "=\"" + word.data() + "\"";
	
			try
			{
				const TByte __key = std::toupper(word.at(0));
				JoinThread jthread(&TDictionary::_search, this, __key, __clause);
			}
			catch (const std::exception& e)
			{
				printerr(e.what())
			}
			return m_resultMap;
		}
		
		void
		TDictionary::_search(TByte key, TDatabaseManager::TClause clause)
		{
			if (not m_resultMap.empty())
				m_resultMap.clear();
			
			std::lock_guard lockGuard(m_mutex);
			TDatabaseManager::TResultSetPtr __resultSetPtr {};
			
			try
			{
				__resultSetPtr = m_databaseManager->selectColumn(m_table, m_column);
			} catch (sql::SQLException& e)
			{
				printerr(e.what())
			}
			
			while (__resultSetPtr->next())
			{
				if (__resultSetPtr->getString(1)[0] == key)
				{
					const TString table = __resultSetPtr->getString(1);
					_collect(table, clause);
				}
			}
		}
		
		void TDictionary::_collect(const TString& table, TDatabaseManager::TClause clause)
		{
			auto __cols = m_databaseManager->getColumns(table);
			
			auto __resultSetPtr = m_databaseManager->selectColumnsWhere(table, __cols, clause);
			
			while (__resultSetPtr->next())
			{
				for (size_t i = 0; i < __cols.size(); ++i)
				{
					auto __colValue = __resultSetPtr->getString(i + 1);
					if (not __colValue.length())
						break;
					
					if (auto __pos = m_resultMap.find(__cols.at(i)); __pos != m_resultMap.end())
						__pos->second.push_back(__colValue);
					else
						m_resultMap.emplace_hint(
								m_resultMap.cend(),
								__cols.at(i), std::vector<TString> { __colValue });
				}
			}
		}
	}
}
