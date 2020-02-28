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
		
		class dxDEPRECATED LanguageBook
		{
		public:
			LanguageBook();
			
			~LanguageBook();
			
			void insert(const TString& language, const TString& id);
			
			TUString getView(TString language);
			
			const TCodePoint* getView(TUString language);
		
		private:
			std::list<Alphabet*> m_alphabets;
			
			std::map<TString, const TCodePoint*> m_langdefs;
		};
	}
}
