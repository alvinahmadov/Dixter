// Components for manipulating non-owning sequences of characters -*- C++ -*-

// Copyright (C) 2013-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.#ifndef _DIXTER_STRINGVIEW_H_

#pragma once

#include "Macros.hpp"

#ifndef HAVE_CXX17

#include <limits>
#include <iosfwd>
#include <bits/char_traits.h>
#include <bits/functional_hash.h>
#include <bits/range_access.h>

namespace Dixter
{
	template<typename CharT, typename Traits = std::char_traits<CharT>>
	class string_view
	{
	public:
		// types
		using traits_type = Traits;
		using value_type = CharT;
		using pointer = const CharT*;
		using const_pointer = const CharT*;
		using reference = const CharT&;
		using const_reference = const CharT&;
		using const_iterator = const CharT*;
		using iterator = const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using reverse_iterator = const_reverse_iterator;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		static constexpr size_type npos = size_type(-1);
		
		constexpr
		string_view() noexcept
				: m_length{0}, m_string{nullptr}
		{ }
		
		constexpr
		string_view(const string_view&) noexcept = default;
		
		constexpr
		string_view(const CharT* str) noexcept
				: m_length{str == nullptr ? 0 : traits_type::length(str)},
				  m_string{str}
		{ }
		
		constexpr
		string_view(const std::basic_string<CharT>& str) noexcept
				: m_length{str.empty() ? 0 : str.length()},
				  m_string{str.c_str()}
		{ }
		
		constexpr 
		string_view(const CharT* str, size_type len) noexcept 
				: m_length{len}, m_string{str}
		{ }
		
		constexpr string_view&
		operator=(const string_view& other) noexcept
		{
			if (this != &other)
			{
				delete m_string;
				memmove(m_string, other.m_string, other.m_length);
				m_length = other.m_length;
			}
			return *this;
		};
		
		// [string.view.iterators], iterators
		
		constexpr const_iterator
		begin() const noexcept
		{ return this->m_string; }
		
		 const_iterator
		end() const noexcept
		{ return this->m_string + this->m_length; }
		
		constexpr const_iterator
		cbegin() const noexcept
		{ return this->m_string; }
		
		constexpr const_iterator
		cend() const noexcept
		{ return this->m_string + this->m_length; }
		
		constexpr const_reverse_iterator
		rbegin() const noexcept
		{ return const_reverse_iterator(this->end()); }
		
		constexpr const_reverse_iterator
		rend() const noexcept
		{ return const_reverse_iterator(this->begin()); }
		
		constexpr const_reverse_iterator
		crbegin() const noexcept
		{ return const_reverse_iterator(this->end()); }
		
		constexpr const_reverse_iterator
		crend() const noexcept
		{ return const_reverse_iterator(this->begin()); }
		
		// [string.view.capacity], capacity
		
		constexpr size_type
		size() const noexcept
		{ return this->m_length; }
		
		constexpr size_type
		length() const noexcept
		{ return m_length; }
		
		constexpr size_type
		max_size() const noexcept
		{
			return (npos - sizeof(size_type) - sizeof(void*))
				   / sizeof(value_type) / 4;
		}
		
		constexpr bool
		empty() const noexcept
		{ return this->m_length == 0; }
		
		// [string.view.access], m_element access
		
		constexpr const CharT&
		operator[](size_type pos) const noexcept
		{
			return *(this->m_string + pos);
		}
		
		constexpr const CharT&
		at(size_type pos) const
		{
			if (pos >= m_length)
				__throw_out_of_range_fmt(__N("string_view::at: pos "
											 "(which is %zu) >= this->size() "
											 "(which is %zu)"), pos, this->size());
			return *(this->m_string + pos);
		}
		
		constexpr const CharT&
		front() const noexcept
		{
			return *this->m_string;
		}
		
		constexpr const CharT&
		back() const noexcept
		{
			return *(this->m_string + this->m_length - 1);
		}
		
		constexpr const CharT*
		data() const noexcept
		{ return this->m_string; }
		
		// [string.view.modifiers], modifiers:
		
		void
		remove_prefix(size_type n) noexcept
		{
			__glibcxx_assert(this->m_length >= __n);
			this->m_string += n;
			this->m_length -= n;
		}
		
		void
		remove_suffix(size_type n) noexcept
		{ this->m_length -= n; }
		
		void
		swap(string_view& sv) noexcept
		{
			std::swap(*this, sv);
		}
		
		
		// [string.view.ops], string operations:
		
		size_type
		copy(CharT* str, size_type n, size_type pos = 0) const
		{
			__glibcxx_requires_string_len(str, __n);
			pos = f_check(pos, "string_view::copy");
			const size_type __rlen = std::min(n, m_length - pos);
			for (auto __begin = this->m_string + pos,
						 __end = __begin + __rlen; __begin != __end;)
				*str++ = *__begin++;
			return __rlen;
		}
		
		constexpr string_view
		substr(size_type pos, size_type n = npos) const noexcept(false)
		{
			pos = f_check(pos, "string_view::substr");
			const size_type __rlen = std::min(n, m_length - pos);
			return string_view{m_string + pos, __rlen};
		}
		
		constexpr int
		compare(string_view str) const noexcept
		{
			const size_type __rlen = std::min(this->m_length, str.m_length);
			int __ret = traits_type::compare(this->m_string, str.m_string, __rlen);
			if (__ret == 0)
				__ret = s_compare(this->m_length, str.m_length);
			return __ret;
		}
		
		constexpr int
		compare(size_type pos1, size_type __n1, string_view str) const
		{ return this->substr(pos1, __n1).compare(str); }
		
		constexpr int
		compare(size_type pos1, size_type n1,
				string_view str, size_type pos2, size_type n2) const
		{
			return this->substr(pos1, n1).compare(str.substr(pos2, n2));
		}
		
		constexpr int
		compare(const CharT* str) const noexcept
		{ return this->compare(string_view{str}); }
		
		constexpr int
		compare(size_type pos1, size_type n1, const CharT* str) const
		{ return this->substr(pos1, n1).compare(string_view{str}); }
		
		constexpr int
		compare(size_type pos1, size_type n1,
				const CharT* str, size_type n2) const noexcept(false)
		{
			return this->substr(pos1, n1)
					.compare(string_view(str, n2));
		}
		
		constexpr size_type
		find(string_view str, size_type pos = 0) const noexcept
		{ return this->find(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		find(CharT c, size_type pos = 0) const noexcept;
		
		constexpr size_type
		find(const CharT* str, size_type pos, size_type n) const noexcept;
		
		constexpr size_type
		find(const CharT* str, size_type pos = 0) const noexcept
		{ return this->find(str, pos, traits_type::length(str)); }
		
		constexpr size_type
		rfind(string_view str, size_type pos = npos) const noexcept
		{ return this->rfind(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		rfind(CharT c, size_type pos = npos) const noexcept;
		
		constexpr size_type
		rfind(const CharT* str, size_type pos, size_type n) const noexcept;
		
		constexpr size_type
		rfind(const CharT* str, size_type pos = npos) const noexcept
		{ return this->rfind(str, pos, traits_type::length(str)); }
		
		constexpr size_type
		find_first_of(string_view str, size_type pos = 0) const noexcept
		{ return this->find_first_of(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		find_first_of(CharT c, size_type pos = 0) const noexcept
		{ return this->find(c, pos); }
		
		constexpr size_type
		find_first_of(const CharT* str, size_type pos, size_type n) const noexcept;
		
		constexpr size_type
		find_first_of(const CharT* str, size_type pos = 0) const noexcept
		{ return this->find_first_of(str, pos, traits_type::length(str)); }
		
		constexpr size_type
		find_last_of(string_view str,
					 size_type pos = npos) const noexcept
		{ return this->find_last_of(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		find_last_of(CharT c, size_type pos=npos) const noexcept
		{ return this->rfind(c, pos); }
		
		constexpr size_type
		find_last_of(const CharT* str, size_type pos,
					 size_type __n) const noexcept;
		
		constexpr size_type
		find_last_of(const CharT* str, size_type pos = npos) const noexcept
		{ return this->find_last_of(str, pos, traits_type::length(str)); }
		
		constexpr size_type
		find_first_not_of(string_view str,
						  size_type pos = 0) const noexcept
		{ return this->find_first_not_of(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		find_first_not_of(CharT c, size_type pos = 0) const noexcept;
		
		constexpr size_type
		find_first_not_of(const CharT* str,
						  size_type pos, size_type __n) const noexcept;
		
		constexpr size_type
		find_first_not_of(const CharT* str, size_type pos = 0) const noexcept
		{
			return this->find_first_not_of(str, pos,
										   traits_type::length(str));
		}
		
		constexpr size_type
		find_last_not_of(string_view str,
						 size_type pos = npos) const noexcept
		{ return this->find_last_not_of(str.m_string, pos, str.m_length); }
		
		constexpr size_type
		find_last_not_of(CharT c, size_type pos = npos) const noexcept;
		
		constexpr size_type
		find_last_not_of(const CharT* str, size_type pos, size_type n) const noexcept;
		
		constexpr size_type
		find_last_not_of(const CharT* str,
						 size_type pos = npos) const noexcept
		{
			return this->find_last_not_of(str, pos,
										  traits_type::length(str));
		}
		
		constexpr size_type
		f_check(size_type pos, const char* s) const noexcept(false)
		{
			if (pos > this->size())
				throw_out_of_range_fmt(__N("%s: pos (which is %zu) > "
											 "this->size() (which is %zu)"),
										 s, pos, this->size());
			return pos;
		}
		
		// NB: f_limit doesn't check for a bad pos value.
		constexpr size_type
		f_limit(size_type pos, size_type off) const noexcept
		{
			const bool testoff =  off < this->size() - pos;
			return testoff ? off : this->size() - pos;
		}
	
	private:
		
		static constexpr int
		s_compare(size_type n1, size_type n2) noexcept
		{
			const difference_type diff = n1 - n2;
			if (diff > std::numeric_limits<int>::max())
				return std::numeric_limits<int>::max();
			if (diff < std::numeric_limits<int>::min())
				return std::numeric_limits<int>::min();
			return static_cast<int>(diff);
		}
		
		size_t	        m_length;
		const CharT*    m_string;
	};
	
	// [string.view.comparison], non-member string_view comparison function
	
	namespace detail
	{
		// Identity transform to create a non-deduced context, so that only one
		// argument participates in template argument deduction and the other
		// argument gets implicitly converted to the deduced type. See n3766.html.
		template<typename _Tp>
		using idt = typename std::common_type<_Tp>::type;
	}
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator==(string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.size() == y.size() && x.compare(y) == 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator==(string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return x.size() == y.size() && x.compare(y) == 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator==(detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.size() == y.size() && x.compare(y) == 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator!=(string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return !(x == y); }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator!=(string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return !(x == y); }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator!=(detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return !(x == y); }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator< (string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) < 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator< (string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return x.compare(y) < 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator< (detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) < 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator> (string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) > 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator> (string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return x.compare(y) > 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator> (detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) > 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator<=(string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) <= 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator<=(string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return x.compare(y) <= 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator<=(detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) <= 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator>=(string_view<_CharT, _Traits> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) >= 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator>=(string_view<_CharT, _Traits> x,
			   detail::idt<string_view<_CharT, _Traits>> y) noexcept
	{ return x.compare(y) >= 0; }
	
	template<typename _CharT, typename _Traits>
	constexpr bool
	operator>=(detail::idt<string_view<_CharT, _Traits>> x,
			   string_view<_CharT, _Traits> y) noexcept
	{ return x.compare(y) >= 0; }
	
	// [string.view.io], Inserters and extractors
	template<typename CharT, typename Traits>
	inline std::basic_ostream<CharT, Traits>&
	operator<<(std::basic_ostream<CharT, Traits>& os,
			   string_view<CharT,Traits> str)
	{ return ostream_insert(os, str.data(), str.size()); }
	
	
	// [string.view.hash], hash support:

} // namespace dix

#else

#include <string_view>

#endif // !_GLIBCXX_STRING_VIEW
