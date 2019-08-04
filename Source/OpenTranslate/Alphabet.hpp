/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
 
#pragma once


#include "Unicode.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		class Alphabet
		{
		public:
			Alphabet(const string_t& languageName, const code_point_t* codePoints);
			
			~Alphabet();
			
			bool contains(const string_t& languageName);
			
			const string_t& getLanguageName() const;
			
			const code_point_t* getCodePoints();
			
			size_t getSize() const;
			
			bool operator==(const Alphabet& other);
			
			bool operator!=(const Alphabet& othr);
		
		private:
			string_t m_lang;
			
			const code_point_t* m_pCodePoints;
		};
	}
}