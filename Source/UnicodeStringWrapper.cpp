/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "UnicodeStringWrapper.hpp"
#include <unicode/unistr.h>
#include <unicode/ustdio.h>

#include <QString>
#include <iostream>
#include <unicode/ustring.h>

namespace Dixter
{
	
	std::ostream& operator<<(std::ostream& out, const UnicodeStringWrapper& us)
	{
		out << us.asUTF8();
		return out;
	}
	
	std::istream& operator>>(std::istream& in, UnicodeStringWrapper& usw)
	{
		std::string s { };
		usw.clear();
		in >> s;
		usw.fromUTF8(s);
		return in;
	}
	
	UnicodeStringWrapper::UnicodeStringWrapper()
			: icu::UnicodeString()
	{ }
	
	UnicodeStringWrapper::UnicodeStringWrapper(const QString& string)
			: icu::UnicodeString(string.toStdU16String().data())
	{ }
	
	UnicodeStringWrapper::UnicodeStringWrapper(const std::string& string)
			: icu::UnicodeString(string.data())
	{ }
	
	UnicodeStringWrapper::UnicodeStringWrapper(const std::wstring& string)
			: icu::UnicodeString(reinterpret_cast<const char16_t*>(string.data()))
	{ }
	
	UnicodeStringWrapper::UnicodeStringWrapper(const char* string)
			: icu::UnicodeString(string)
	{
	}
	
	UnicodeStringWrapper::UnicodeStringWrapper(const icu::UnicodeString& string)
			: icu::UnicodeString(string)
	{
	}
	
	const char* UnicodeStringWrapper::c_str() const
	{
		return asUTF8().c_str();
	}
	
	std::string UnicodeStringWrapper::asUTF8() const
	{
		std::string sc { };
		toUTF8String(sc);
		return sc;
	}
	
	QString UnicodeStringWrapper::asCustom() const
	{
		auto buffer = asUTF8();
		auto customString = QString::fromStdString(buffer);
		return customString;
	}
	
	UnicodeStringWrapper& UnicodeStringWrapper::append(uint32_t i)
	{
		icu::UnicodeString::append(static_cast<UChar32>(i));
		return *this;
	}
	
	UnicodeStringWrapper& UnicodeStringWrapper::append(const std::string& s)
	{
		icu::UnicodeString::append(s.data());
		return *this;
	}
	
	UnicodeStringWrapper& UnicodeStringWrapper::append(const char16_t* s)
	{
		icu::UnicodeString::append(s, 0, u_strlen_60(s));
		return *this;
	}
	
	UnicodeStringWrapper&
	UnicodeStringWrapper::append(const UnicodeStringWrapper& s)
	{
		icu::UnicodeString::append(s);
		return *this;
	}
	
	UnicodeStringWrapper&
	UnicodeStringWrapper::operator=(const std::string& s)
	{
		fromUTF8(s);
		return *this;
	}
	
	UnicodeStringWrapper&
	UnicodeStringWrapper::operator+=(const std::string& s)
	{
		append(s);
		return *this;
	}
	
	UnicodeStringWrapper& UnicodeStringWrapper::operator<<(int v)
	{
		icu::UnicodeString::append(std::to_string(v).data());
		return *this;
	}
	
	UnicodeStringWrapper
	UnicodeStringWrapper::operator<<(const char16_t* v) const
	{
		icu::UnicodeString clone = UnicodeString(*this);
		clone.append(v);
		return clone;
	}
	
	UnicodeStringWrapper
	UnicodeStringWrapper::operator<<(const UnicodeStringWrapper& v) const
	{
		icu::UnicodeString clone = UnicodeString(*this);
		clone.append(v);
		return clone;
	}
	
	bool UnicodeStringWrapper::operator==(const UnicodeStringWrapper& other) const
	{
		return icu::UnicodeString::operator==(other);
	}
	
	bool UnicodeStringWrapper::operator<(const UnicodeStringWrapper& other) const
	{
		return icu::UnicodeString::operator<(other);
	}
	
	bool UnicodeStringWrapper::operator>(const UnicodeStringWrapper& other) const
	{
		return icu::UnicodeString::operator>(other);
	}
	
	bool UnicodeStringWrapper::operator<=(const UnicodeStringWrapper& other) const
	{
		return icu::UnicodeString::operator<=(other);
	}
	
	bool UnicodeStringWrapper::operator>=(const UnicodeStringWrapper& other) const
	{
		return icu::UnicodeString::operator>=(other);
	}
	
	bool UnicodeStringWrapper::empty() const
	{
		return isEmpty();
	}
	
	void UnicodeStringWrapper::clear()
	{
		remove();
	}
	
	int UnicodeStringWrapper::compare(const std::string& s) const
	{
		return icu::UnicodeString::compare(fromUTF8(s));
	}
	
	int UnicodeStringWrapper::compare(const UnicodeStringWrapper& s) const
	{
		return icu::UnicodeString::compare(s);
	}
	
	UnicodeStringWrapper& UnicodeStringWrapper::fromUTF32(const UChar32 *src, size_t codePointLen)
	{
		UChar * dst = new UChar[codePointLen];
		UErrorCode errorCode {};
		u_strFromUTF32_60(dst, codePointLen, NULL, src, codePointLen, &errorCode);
		const UChar* rs = dst;
		clear();
		replace(0, length(), rs, 0, codePointLen);
		return *this;
	}
}