/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <cppconn/resultset.h>
#include "Dictionary.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		Dictionary::Dictionary(Database::Manager* manager)
				: m_databaseManager {manager}
		{
		
		}
		
		Dictionary::~Dictionary()
		{
		
		}
		
		std::multimap<string_t, string_t>
		Dictionary::search(const string_t& word, string_t column, bool asRegex)
		{
			std::multimap<string_t, string_t> resultMap {};
			char firstLetter = std::toupper(word.at(0));
			auto rs = m_databaseManager->selectColumn("tables", "original_value");
			std::list<string_t> tableData {};
			
			while (rs->next())
			{
				if (rs->getString(1)[0] == firstLetter)
				{
					const string_t table = rs->getString(1);
					auto cols = m_databaseManager->getColumns(table);
					sql::ResultSet* resultSet {nullptr};
					
					if (asRegex)
					{
						resultSet = m_databaseManager->selectColumnsWhere(table, cols, column + " LIKE \"" + word + "%\"");
					} else
						resultSet = m_databaseManager->selectColumnsWhere(table, cols, column + "=\"" + word + "\"");
							
					
					while(resultSet->next())
					{
						for (size_t i = 0; i < cols.size(); ++i)
						{
							auto colValue = resultSet->getString(i + 1);
							if (colValue.length() == 0)
								break;
							resultMap.emplace(cols.at(i), colValue);
						}
					}
				}
			}
			return resultMap;
		}
	}
}
