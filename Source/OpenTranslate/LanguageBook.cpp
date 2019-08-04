/**
 *  File LanguageCollection.cpp
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "LanguageBook.hpp"
#include "Macros.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		LanguageBook::LanguageBook()
				: m_alphabets { },
				  m_langdefs { unicode::langdefs }
		{ }
		
		LanguageBook::~LanguageBook()
		{
			for (auto& alpha : m_alphabets)
				SAFE_RELEASE(alpha)
		}
		
		void LanguageBook::insert(const string_t& language, const string_t& id)
		{
			m_alphabets.push_back(new Alphabet(language, m_langdefs.find(id)->second));
		}
		
		ustring_t
		LanguageBook::getView(string_t language)
		{
			ustring_t view { };
			for (const auto& alpha : m_alphabets)
			{
				if (alpha->contains(language))
				{
					auto chrs = alpha->getCodePoints();
					
					for (size_t i = 0; i < alpha->getSize(); ++i)
						view.append(chrs[i]);
				}
			}
			return view;
		}
		
		
		const code_point_t* LanguageBook::getView(ustring_t language)
		{
			for (const auto& alpha : m_alphabets)
			{
				if (alpha->contains(language.asUTF8()))
				{
					return alpha->getCodePoints();
				}
			}
		}
	}
}