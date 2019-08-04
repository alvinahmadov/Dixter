/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <iostream>
#include <algorithm>
#include <limits>
#include <boost/format.hpp>
#include <list>
#include <cstring>
#include <unicode/ustring.h>
#include <clocale>

#ifdef HAVE_CXX17

#include <string_view>
#include <any>

#else

#include <boost/any.hpp>
#include "StringView.hpp"

#endif

#include "Types.hpp"

namespace Dixter
{
	namespace Utilities
	{
		namespace Algorithms
		{
			template<class Container,
			         typename Functor>
			Functor& forEach(Container& container, Functor functor)
			{
				return std::for_each(container.begin(), container.end(), functor);
			};
			
			template<class Container,
			         typename Functor>
			Functor& forEach(const Container& container, Functor functor)
			{
				return std::for_each(container.begin(), container.end(), functor);
			};
			
			template<class Container,
			         typename Functor>
			Functor& forEach(Container* container, Functor functor)
			{
				return std::for_each(container->begin(), container->end(), functor);
			};
			
			template<typename Number,
			         typename Functor>
			void forEach(Number& start, const Number& end, Functor functor)
			{
				for (start = 0; start < end; start++)
					functor(start);
			};
			
			template<class Container,
			         typename IIter = typename Container::const_iterator,
			         typename OIter = typename Container::iterator>
			OIter copyAll(const Container& input, Container& output)
			{
				if (input.size() != output.size())
				{
					return OIter();
				}
				OIter __result = std::copy(input.begin(), input.end(), output.begin());
				return __result;
			}
			
			template<typename Container,
			         typename Functor,
			         typename Iterator = typename Container::iterator>
			void foreachCompound(Container& container1, Container& container2, Functor functor)
			{
				// if (container1.size() != container2.size())
				// {
				// 	return;
				// }
				Iterator __lFirst = container1.begin();
				Iterator __rFirst = container2.begin();
				Iterator __lLast = container1.end();
				Iterator __rLast = container2.end();
				for (; __lFirst != __lLast && __rFirst != __rLast; ++__rFirst, ++__lFirst)
				{
					functor(*__lFirst, *__rFirst);
				}
			}
			
			template<typename Container1,
			         typename Container2,
			         typename Functor,
			         typename IIterator = typename Container1::const_iterator,
			         typename OIterator = typename Container2::iterator>
			void foreachCompound(Container1& container1, Container2& container2, Functor functor)
			{
				if (size_t(container1.size()) != size_t(container2.size()))
					return;
				IIterator __lFirst = container1.begin();
				OIterator __rFirst = container2.begin();
				IIterator __lLast = container1.end();
				OIterator __rLast = container2.end();
				for (; __lFirst != __lLast && __rFirst != __rLast; ++__rFirst, ++__lFirst)
				{
					functor(*__lFirst, *__rFirst);
				}
			}
			
			template<typename Container,
			         typename Functor,
			         typename Iterator = typename Container::iterator>
			void iterateParallel(Container* container1, Container* container2, Functor functor)
			{
				
				Iterator __lFirst = container1->begin();
				Iterator __rFirst = container2->begin();
				Iterator __lLast = container1->begin();
				Iterator __rLast = container2->begin();
				for (; __lFirst != __lLast && __rFirst != __rLast; ++__rFirst, ++__lFirst)
				{
					functor(__lFirst, __rFirst);
				}
			}
		}
		namespace Strings
		{
			// Utilities to work with string types
			// TODO: Error for now
			/**
			 * @brief Create string representation of data in container separated by explodeBy
			 * @tparam Container The type of container from which generated data representation
			 * @tparam StringType The type of the string, unicode or not
			 * @tparam CharType The type of the char by which data exploded
			 * @param container Container holding data values
			 * @param separator Character by which elements separated in representation
			 * @return string representation
			 * */
			template<class Container,
			         class StringType = typename Container::value_type,
			         class CharType = typename StringType::value_type>
			StringType join(const Container& container, CharType separator = CharType(','))
			{
				StringType __result { };
				size_t __index { };
				size_t __start { };
				for (const StringType& __val : container)
				{
					while (true)
					{
						if (__val.at(__index) == separator)
						{
							__result.append(__val.substr(__start, __index - __start));
							__start = ++__index;
						}
						if (++__index == __val.size() - 1)
						{
							__result.append(__val.substr(__start, __val.size() - __start));
							break;
						}
					}
				}
				return __result;
			}
			
			/** @brief Generate a container from string representation str of data separated by delimiter
			  * @tparam Container The type of container from which generated data representation
			  * @tparam StringType The type of the string, unicode or not
			  * @tparam CharType The type of the char by which data exploded
			  * @param str String which holds separate values
			  * @param separator Character by which the data in string is sepearated
			  * @return Newly generated container (list, vector, stack)
			  */
			template<class Container, class StringType,
			         class CharType = typename StringType::value_type>
			Container split(const StringType& str, CharType separator = CharType(','))
			{
				size_t __start { };
				size_t __pos { };
				Container __container = Container();
				while (__pos != StringType::npos)
				{
					__pos = str.find(separator, __start);
					__container.push_back(str.substr(__start, __pos - __start));
					__start = __pos + 1;
				}
				
				return __container;
			}
			
			template<class String,
			         typename CharType = typename String::value_type>
			void trim(String& str, const CharType charCode = ' ', bool symmetrical = true)
			{
				str = str.erase(str.find(charCode), 1);
				if (symmetrical)
				{
					str = str.erase(str.find(charCode, str.size() - 1), 1);
				}
			}
			
			template<typename ... Strings>
			string_t concat(Strings... args)
			{
				string_t __result { };
				std::initializer_list<string_t> __argList { std::forward<Strings>(args)... };
				for (const auto& value : __argList)
					__result.append(value);
				
				return __result;
			}
			
			template<typename ... Args, typename Char = typename string_t::value_type>
			string_t buildPath(Char separator, const Args& ... args)
			{
				string_t __res { };
				std::initializer_list<string_t> __strings { args... };
				size_t __index = 0;
				
				if (sizeof...(Args) > 1)
				{
					for (const auto& str : __strings)
					{
						__res.append(str);
						if (++__index != __strings.size())
						{
							__res += separator;
						}
					}
				} else
				{
					__res += separator;
					__res += *__strings.end();
				}
				return __res;
			}
			
			template<class String, typename ... Args, typename Char = typename String::value_type>
			void buildPath(String& returnValue, Char separator, const Args& ... args)
			{
				std::list<String> __strings { args... };
				size_t __index = 0;
				
				if (sizeof...(Args) > 1)
				{
					for (const auto& str : __strings)
					{
						returnValue.append(str);
						if (++__index != __strings.size())
						{
							returnValue += separator;
						}
					}
				} else
				{
					returnValue.append(1, separator).append(__strings.back());
				}
			}
			
			template<typename String, typename Char = typename String::value_type>
			void encloseWith(String& returnValue, Char startChar, Char endChar)
			{
				returnValue.insert(0, 1, startChar);
				returnValue.insert(returnValue.length(), 1, endChar);
			}
			
			template<class Container,
			         typename ValueType = typename Container::value_type>
			string_t toString(const Container& container, char delim = ',')
			{
				string_t __result;
				bool __isArithmetic;
				typedef ValueType value_type;
				typedef typename string_t::size_type size_type;
				
				#ifdef HAVE_CXX17
				__isArithmetic = std::is_arithmetic_v<value_type>;
				#elif defined(HAVE_CXX14) || defined(HAVE_CXX11)
				__isArithmetic = std::is_arithmetic<value_type>::value;
				#endif
				
				size_type __index { };
				
				__result.push_back('[');
				for (auto __containerItem : container)
				{
					if (__isArithmetic)
					{
						auto strItem = std::to_string(__containerItem);
						__result.append(strItem);
					}
					/*
					else
					{
						if (__containerItem.size() > 1)
							__result.append(__containerItem);
						else
							__result.push_back(1, __containerItem);
					}
					*/
					
					if ((++__index != container.size()) && container.size() > 1)
					{
						__result.append(1, delim).push_back(' ');
					}
				}
				__result.push_back(']');
				return __result;
			}
			
			wstring_t toWstring(const std::string& str);
			
			int wstrToInt(const std::wstring& value);
			
			//TODO: Fix for integral and string types
			#ifdef USE_TO_STRING_CONVERSION
			template<class Map,
				typename KeyType    = typename Map::key_type,
				typename MappedType = typename Map::mapped_type>
		string_t toString(const Map& map,
						  char pairDelim, char delim = ',')
		{
			string_t __result;
			bool keyIsArithmetic{};
			bool MappedIsArithmetic{};
			typedef typename string_t::size_type size_type;
			#if HAVE_CXX17
			typedef std::remove_reference_t<std::remove_const_t<KeyType>>       non_const_key_t;
			typedef std::remove_reference_t<std::remove_const_t<MappedType>>    non_const_mapped_t;
			keyIsArithmetic    = std::is_arithmetic_v<non_const_key_t>;
			MappedIsArithmetic = std::is_arithmetic_v<non_const_mapped_t>;
			#elif HAVE_CXX14 || HAVE_CXX11
			typedef std::remove_reference_t<std::remove_const_t<KeyType>>       non_const_key_t;
			typedef std::remove_reference_t<std::remove_const_t<MappedType>>    non_const_mapped_t;
			keyIsArithmetic    = std::is_arithmetic<non_const_key_t>::value;
			MappedIsArithmetic = std::is_arithmetic<non_const_mapped_t>::value;
			#elif defined(HAVE_LEGACY)
			typedef typename ArithmeticType non_const_rvalue_type;
			#endif

			size_type __index {};
			__result.push_back('[');
			for (auto __pair : map)
			{
				if (keyIsArithmetic)
				{
					auto keyItem = std::to_string(__pair.first);
					__result.push_back('{');
					__result.append(keyItem).append(1, pairDelim);
				} else
				{
					__result += __pair.first;
					__result.append(1, pairDelim);
				}
				__result.push_back(' ');
				
				if (MappedIsArithmetic)
				{
					std::cout << "Arithmetic: ";
					std::cout << __pair.second << std::endl;
					auto mappedItem = to_string(__pair.second);
//					auto mappedItem = std::to_string(__pair.second);
//					__result += __pair.second;
					__result.append(mappedItem);
					__result.push_back('}');
				} else
				{
					std::cout << "Not arithmetic: ";
					std::cout << __pair.second << std::endl;
					__result.append(to_string(__pair.second));
//					__result += __pair.second;
				}
				
				if ((++__index != map.size()) && map.size() > 1)
					__result.append(1, delim).push_back(' ');
			}
			__result.push_back(']');
			return __result;
		}
			#endif
			
			template<typename String1, typename String2>
			String1 convertString(const String2& string1)
			{
				return String1 { string1.data() };
			}
			
			template<typename ArithmeticType>
			string_t to_string(ArithmeticType value)
			{
				bool __isSame { };
				#ifdef HAVE_CXX17
				typedef std::remove_reference_t<std::remove_const_t<ArithmeticType>> non_const_rvalue_t;
				__isSame = std::is_same_v<string_t, ArithmeticType>;
				#elif defined(HAVE_CXX14) || defined(HAVE_CXX11)
				typedef typename std::remove_reference<typename std::remove_const<ArithmeticType>::type>::type non_const_rvalue_t;
			__isSame = std::is_same<string_t, ArithmeticType>::value;
			#elif defined(HAVE_LEGACY)
			typedef typename ArithmeticType non_const_rvalue_type;
				#endif
				auto nonConstRvalue = static_cast<non_const_rvalue_t>(value);
				if (__isSame)
				{
					return nonConstRvalue;
				} else
				{
					return std::to_string(nonConstRvalue);
				}
				return "";
			}
			
			int uStringToInt(const ustring_t& value);
			
			ustring_t intToUstring(int value);
		}
	}
}