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
				  m_langdefs { Unicode::langdefs }
		{ }
		
		LanguageBook::~LanguageBook()
		{
			for (auto& alpha : m_alphabets)
				delete alpha;
		}
		
		void LanguageBook::insert(const TString& language, const TString& id)
		{
			m_alphabets.push_back(new Alphabet(language, m_langdefs.find(id)->second));
		}
		
		TUString
		LanguageBook::getView(TString language)
		{
			TUString view { };
			for (const auto& alpha : m_alphabets)
			{
				if (alpha->contains(language))
				{
					auto chrs = alpha->getCodePoints();
					
					for (TSize i = 0; i < alpha->getSize(); ++i)
						view.append(chrs[i]);
				}
			}
			return view;
		}
		
		
		const TCodePoint* LanguageBook::getView(TUString language)
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