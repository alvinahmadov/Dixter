/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Alphabet.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		Alphabet::Alphabet(const string_t& languageName, const code_point_t* codePoints)
				: m_lang { languageName },
				  m_pCodePoints { codePoints }
		{ }
		
		Alphabet::~Alphabet()
		{ }
		
		bool Alphabet::contains(const string_t& languageName)
		{
			return ((m_lang == languageName) && (!m_lang.compare(languageName)));
		}
		
		const string_t&
		Alphabet::getLanguageName() const
		{
			return m_lang;
		}
		
		const code_point_t*
		Alphabet::getCodePoints()
		{
			return m_pCodePoints;
		}
		
		size_t Alphabet::getSize() const
		{
			size_t index { };
			while (m_pCodePoints[++index] != '\0')
			{ }
			return index;
		}
		
		bool Alphabet::operator==(const Alphabet& othr)
		{
			return (m_lang.compare(othr.m_lang) && m_pCodePoints == othr.m_pCodePoints);
		}
		
		bool Alphabet::operator!=(const Alphabet& othr)
		{
			return !operator==(othr);
		}
	}
}