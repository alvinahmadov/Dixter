/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <cstdint>
#include <memory>
#include "UnicodeString.hpp"

namespace Dixter
{
	using TByte          = char;
	#ifdef USE_WCHART16
	using TByte16        = char16_t;
	#else
	using TByte16 = TByte;
	#endif
	#ifdef USE_WCHART32
	using TByte32        = char32_t;
	#else
	using TByte32        = TByte;
	#endif
	using TSByte        = signed char;
	using TUByte        = unsigned char;
	using TByteArray    = const TByte*;
#ifdef HAS_WCHAR
	using TWByte        = wchar_t;
#else
	using TWByte        = TByte;
#endif
	using Int8          = signed char;
	using Int16         = signed short;
	using Int32         = int;
	using Int64         = int64_t;
	using UInt8         = unsigned char;
	using UInt16        = unsigned short;
	using UInt32        = unsigned int;
	using UInt64        = uint64_t;
	using TSize         = size_t;
	using Real          = float;
	using Real32        = double;
	using Real64        = long double;
	using TCodePoint    = char32_t;
#ifndef WIN32
	using TId           = id_t;
#else
	using UInt = UInt32;
	using TId = uint;
#endif
	using TString       = std::string;
	using TWString      = std::wstring;
	using TUString      = TUnicodeString;
	using TStringPair	= std::pair<TString, TString>;
} // namespace Dixter

#ifdef HAVE_CXX17

#	include <string_view>

namespace Dixter
{
	using TStringView = std::string_view;
#	ifdef HAS_WCHAR
	using TWStringView = std::wstring_view;
#	endif
#	ifdef USE_C99_STDINT_TR1
	using TStringView16 = std::basic_string_view<char16_t>;
	using TStringView32 = std::basic_string_view<char32_t>;
#	endif
} // namespace Dixter

#endif //!HAVE_CXX17