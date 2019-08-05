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
#include "UnicodeStringWrapper.hpp"

#include "StringView.hpp"

namespace Dixter
{
	namespace
	{
		using byte = char;
		#ifdef USE_WCHART16
		using byte16 = char16_t;
		#else
		using byte16 = byte;
		#endif
		#ifdef USE_WCHART32
		using byte32 = char32_t;
		#else
		using byte32 = byte;
		#endif
		using sbyte = signed char;
		using ubyte = unsigned char;
		using bytes = byte*;
		using bytes_const = bytes const*;
		using const_bytes = const byte*;
#ifdef USE_WCHART
		using wbyte = wchar_t;
#else
		using wbyte = byte;
#endif
		using i8 = int8_t;
		using i16 = int16_t;
		using i32 = int32_t;
		using i64 = int64_t;
		using ui8 = uint8_t;
		using ui16 = uint16_t;
		using ui32 = uint32_t;
		using ui64 = uint64_t;
		using r32 = float;
		using d32 = double;
		using d64 = long double;
		using code_point_t = char32_t;
#ifndef WIN32
		using id_type = id_t;
#else
		using uint = ui32;
		using id_type = uint;
#endif
		using pos_t = i32;
		using dimension_t = ui32;
		using coordinate_t = ui32;
		
		using unichar = ui8;
		using string_t = std::string;
		using wstring_t = std::wstring;
		using ustring_t = UnicodeStringWrapper;
		
#ifdef HAVE_CXX17
		using string_view_t = std::basic_string_view<Dixter::byte>;
#       ifdef USE_WCHART
		using wstring_view_t = std::basic_string_view<Dixter::wbyte>;
#       endif
#       ifdef USE_C99_STDINT_TR1
		using u16string_view_t = std::basic_string_view<Dixter::byte16>;
		using u32string_view_t = std::basic_string_view<Dixter::byte32>;
#       endif
#else
		using string_view_t = string_view<Dixter::byte>;
#       ifdef USE_WCHART
		using wstring_view_t = string_view<Dixter::wbyte>;
#       endif

#       ifdef USE_C99_STDINT_TR1
		using u16string_view_t = string_view<Dixter::byte16>;
		using u32string_view_t = string_view<Dixter::byte32>;
#       endif
#endif // HAVE_CXX17
	}
	
	namespace
	{
		template<typename _Tp>
		struct hash;
		
		template<>
		struct hash<string_view_t>
				: public std::__hash_base<size_t, string_view_t>
		{
			size_t
			operator()(const string_view_t& str) const dxDECL_NOEXCEPT
			{ return std::_Hash_impl::hash(str.data(), str.length()); }
		};
		
		template<typename Hash>
		struct is_fast_hash : std::__is_fast_hash<Hash>
		{
		};

#ifdef USE_WCHART
		
		template<>
		struct hash<wstring_view_t>
				: public std::__hash_base<size_t, std::wstring>
		{
			size_t
			operator()(const wstring_view_t& s) const dxDECL_NOEXCEPT
			{
				return std::_Hash_impl::hash(s.data(),
				                             s.length() * sizeof(wchar_t));
			}
		};

#endif
	}
	
	#ifndef HAVE_CXX17
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wliteral-suffix"
	inline namespace literals
	{
		inline namespace string_view_literals
		{
			inline dxCONSTEXPR string_view_t
			operator ""sv(const byte* str, size_t len) dxNOEXCEPT
			{ return string_view_t {str, len}; }

#ifdef USE_WCHART
			
			inline dxCONSTEXPR wstring_view_t
			operator ""sv(const wbyte* str, size_t len) dxNOEXCEPT
			{ return wstring_view_t{str, len}; }

#endif

#ifdef USE_C99_STDINT_TR1
			
			inline dxCONSTEXPR u16string_view_t
			operator ""sv(const byte16* str, size_t len) dxNOEXCEPT
			{ return u16string_view_t {str, len}; }
			
			inline dxCONSTEXPR u32string_view_t
			operator ""sv(const byte32* str, size_t len) dxNOEXCEPT
			{ return u32string_view_t {str, len}; }

#endif

	#pragma GCC diagnostic pop
		} // namespace string_literals
	} // namespace literals
	#endif
}