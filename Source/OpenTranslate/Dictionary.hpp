/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Database/Manager.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		class Dictionary
		{
		public:
			Dictionary(Database::Manager* manager);
			
			~Dictionary();
			
			std::multimap<string_t, string_t>
			search(const string_t& word, string_t column, bool asRegex = false);
		
		private:
			Database::Manager* m_databaseManager;
		};
	}
}