/**
 *  File LanguageCollection.h
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once


#include "Alphabet.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		class LanguageBook
		{
		public:
			LanguageBook();
			
			~LanguageBook();
			
			void insert(const string_t& language, const string_t& id);
			
			ustring_t getView(string_t language);
			
			const code_point_t* getView(ustring_t language);
		
		private:
			std::list<Alphabet*> m_alphabets;
			
			std::map<string_t, const code_point_t*> m_langdefs;
		};
	}
}
