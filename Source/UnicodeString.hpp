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
#include <iostream>
#include "setup.h"

#ifdef HAS_QSTRING
class QString;
#endif

namespace Dixter
{
	class TUnicodeString;
	
	std::ostream&
	operator<<(std::ostream& out, const TUnicodeString& us);
	
	std::istream&
	operator>>(std::istream& in, TUnicodeString& us);
	
	class TUnicodeString : public icu::UnicodeString
	{
	public:
		using TBase = icu::UnicodeString;
		using TSelf = TUnicodeString;
		using value_type = char;
	public:
		TUnicodeString() = default;
		
		TUnicodeString(const TSelf& self) = default;
		
		TSelf& operator=(const TSelf& self) = default;
		
		TUnicodeString(const std::string& string);
		
		TUnicodeString(const TBase& string);
		
		TUnicodeString(const char* string);
		
		#ifdef HAS_QSTRING
		
		TUnicodeString(const QString& string);
		
		QString asCustom() const;
		
		#endif
		
		#ifdef HAS_WCHAR
		
		explicit TUnicodeString(const std::wstring& string);
		
		#endif
		
		TSelf& fromUTF32(const UChar32* src, size_t codePointLen);
		
		std::string asUTF8() const;
		
		TSelf& append(uint32_t i);
		
		TSelf& append(const std::string& s);
		
		TSelf& append(const char16_t* uswd);
		
		TSelf& append(const TSelf& self);
		
		TSelf& prepend(uint32_t i);
		
		TSelf& prepend(const std::string& s);
		
		TSelf& prepend(const char16_t* uswd);
		
		TSelf& prepend(const TSelf& self);
		
		bool empty() const
		{
			return isEmpty();
		}
		
		void clear()
		{
			remove();
		}
		
		int compare(const std::string& s) const
		{
			return TBase::compare(fromUTF8(s));
		}
		
		int compare(const TSelf& self) const
		{
			return TBase::compare(self);
		}
		
		TSelf& operator=(const std::string& s)
		{
			fromUTF8(s);
			return *this;
		}
		
		TSelf& operator+=(const std::string& s)
		{
			append(s);
			return *this;
		}
		
		TSelf operator<<(const TSelf& self) const;
		
		TSelf& operator<<(int v);
		
		TSelf operator<<(const char16_t* v) const;
		
		bool operator==(const TSelf& self) const
		{
			return TBase::operator==(self);
		}
		
		bool operator<(const TSelf& self) const
		{
			return TBase::operator<(self);
		}
		
		bool operator>(const TSelf& self) const
		{
			return TBase::operator>(self);
		}
		
		bool operator<=(const TSelf& self) const
		{
			return TBase::operator<=(self);
		}
		
		bool operator>=(const TSelf& self) const
		{
			return TBase::operator>=(self);
		}
		
		inline operator int() const
		{
			int __nm(std::stoi(asUTF8()));
			return __nm;
		}
		
		inline operator long() const
		{
			long __nm(std::stol(asUTF8()));
			return __nm;
		}
		
		inline operator long long() const
		{
			long long __nm(std::stoll(asUTF8()));
			return __nm;
		}
		
		inline operator unsigned long() const
		{
			unsigned long __nm(std::stoul(asUTF8()));
			return __nm;
		}
		
		inline operator unsigned long long() const
		{
			unsigned long long __nm(std::stoull(asUTF8()));
			return __nm;
		}
		
		inline operator float() const
		{
			float __nm(std::stof(asUTF8()));
			return __nm;
		}
		
		inline operator double() const
		{
			double __nm(std::stod(asUTF8()));
			return __nm;
		}
	};
}