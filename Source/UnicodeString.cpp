/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "setup.h"
#include "UnicodeString.hpp"
#include <unicode/unistr.h>
#if __has_include(<QString>)
#include <QString>
#endif
#include <unicode/ustring.h>

namespace Dixter
{
	
	TUnicodeString::TUnicodeString(const std::string& string)
			: TBase(string.data())
	{ }
	
	#ifdef HAS_QSTRING
	
	TUnicodeString::TUnicodeString(const QString& string)
			: TBase(string.toStdU16String().data())
	{ }
	
	QString TUnicodeString::asCustom() const
	{
		auto buffer = asUTF8();
		auto customString = QString::fromStdString(buffer);
		return customString;
	}
	
	#endif
	
	#ifdef HAS_WCHAR
	
	TUnicodeString::TUnicodeString(const std::wstring& string)
			: TBase(reinterpret_cast<const char16_t*>(string.data()))
	{ }
	
	#endif
	
	TUnicodeString::TUnicodeString(const icu::UnicodeString& string)
			: TBase(string)
	{ }
	
	TUnicodeString::TUnicodeString(const char* string)
			: TBase(string)
	{ }
	
	std::string TUnicodeString::asUTF8() const
	{
		std::string sc { };
		toUTF8String(sc);
		return sc;
	}
	
	TUnicodeString&
	TUnicodeString::append(uint32_t i)
	{
		icu::UnicodeString::append(static_cast<UChar32>(i));
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::append(const std::string& s)
	{
		icu::UnicodeString::append(s.data());
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::append(const char16_t* s)
	{
		icu::UnicodeString::append(s, 0, u_strlen_60(s));
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::append(const TSelf& s)
	{
		icu::UnicodeString::append(s);
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::prepend(uint32_t i)
	{
		icu::UnicodeString::insert(0, static_cast<UChar32>(i));
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::prepend(const std::string& s)
	{
		icu::UnicodeString::append(s.data());
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::prepend(const char16_t* s)
	{
		icu::UnicodeString::insert(0, s);
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::prepend(const TSelf& s)
	{
		icu::UnicodeString::insert(0, s);
		return *this;
	}
	
	TUnicodeString&
	TUnicodeString::fromUTF32(const UChar32 *src, size_t codePointLen)
	{
		UChar * dst = new UChar[codePointLen];
		UErrorCode errorCode {};
		u_strFromUTF32_60(dst, codePointLen, NULL, src, codePointLen, &errorCode);
		const UChar* rs = dst;
		clear();
		replace(0, length(), rs, 0, codePointLen);
		return *this;
	}
	
	TUnicodeString
	TUnicodeString::operator<<(const TUnicodeString& self) const
	{
		auto __clone = TBase(*this);
		__clone.append(self);
		return __clone;
	}
	
	TUnicodeString&
	TUnicodeString::operator<<(int v)
	{
		this->append(std::to_string(v));
		return *this;
	}
	
	TUnicodeString
	TUnicodeString::operator<<(const char16_t* v) const
	{
		auto __clone = TBase(*this);
		__clone.append(v);
		return __clone;
	}
	
	std::ostream&
	operator<<(std::ostream& out, const TUnicodeString& us)
	{
		out << us.asUTF8();
		return out;
	}
	
	std::istream&
	operator>>(std::istream& in, TUnicodeString& us)
	{
		std::string s { };
		us.clear();
		in >> s;
		us.fromUTF8(s);
		return in;
	}
}