/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <unicode/unistr.h>
#include <string>


class QString;

namespace Dixter
{
	class UnicodeStringWrapper;
	
	std::ostream& operator<<(std::ostream& o, const UnicodeStringWrapper& usw);
	
	std::istream& operator>>(std::istream& i, UnicodeStringWrapper& usw);
	
	class UnicodeStringWrapper : public icu::UnicodeString
	{
	public:
		UnicodeStringWrapper();
		
		UnicodeStringWrapper(const QString& string);
		
		UnicodeStringWrapper(const std::string& string);
		
		UnicodeStringWrapper(const std::wstring& string);
		
		UnicodeStringWrapper(const char* string);
		
		UnicodeStringWrapper(const icu::UnicodeString& string);
		
		bool empty() const;
		
		void clear();
		
		int compare(const std::string &) const;
		
		int compare(const UnicodeStringWrapper& usw) const;
		
		UnicodeStringWrapper& fromUTF32(const UChar32 *src, size_t codePointLen);
		
		std::string asUTF8() const;
		
		QString asCustom() const;
		
		UnicodeStringWrapper& append(uint32_t i);
		
		UnicodeStringWrapper& append(const std::string& s);
		
		UnicodeStringWrapper& append(const char16_t*  uswd);
		
		UnicodeStringWrapper& append(const UnicodeStringWrapper& usw);
		
		UnicodeStringWrapper& operator=(const std::string& s);
		
		UnicodeStringWrapper& operator+=(const std::string& s);
		
		UnicodeStringWrapper& operator<<(int );
		
		UnicodeStringWrapper operator<<(const char16_t*) const ;
		
		UnicodeStringWrapper operator<<(const UnicodeStringWrapper& o) const;
		
		bool operator==(const UnicodeStringWrapper& uws) const;
		
		bool operator<(const UnicodeStringWrapper& uws) const;
		
		bool operator>(const UnicodeStringWrapper& uws) const;
		
		bool operator<=(const UnicodeStringWrapper& uws) const;
		
		bool operator>=(const UnicodeStringWrapper& uws) const;
		
	};
}


