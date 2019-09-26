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
#include <sstream>
#include <boost/format.hpp>
#include <list>
#include <cstring>
#include <clocale>

#ifdef HAVE_CXX17

#include <any>

#else

#include <boost/any.hpp>

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
				if (container1.size() != container2.size())
					return;
				Iterator __lFirst = container1.begin();
				Iterator __rFirst = container2.begin();
				Iterator __lLast = container1.end();
				Iterator __rLast = container2.end();
				for (; __lFirst != __lLast and __rFirst != __rLast; ++__rFirst, ++__lFirst)
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
				if (TSize(container1.size()) != TSize(container2.size()))
					return;
				IIterator __lFirst = container1.begin();
				OIterator __rFirst = container2.begin();
				IIterator __lLast = container1.end();
				OIterator __rLast = container2.end();
				for (; __lFirst != __lLast and __rFirst != __rLast; ++__rFirst, ++__lFirst)
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
				for (; __lFirst != __lLast and __rFirst != __rLast; ++__rFirst, ++__lFirst)
				{
					functor(__lFirst, __rFirst);
				}
			}
			
			template<typename TContainer, typename IIterator>
			typename std::iterator_traits<IIterator>::difference_type
			count(IIterator first, IIterator last, const TContainer& container)
			{
				typename std::iterator_traits<IIterator>::difference_type __diff{};
				for (const auto& __v : container)
				{
					__diff += std::count(first, last, __v);
				}
				
				return __diff;
			}
		} // namespace Algorithms
		
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
				TSize __index { };
				TSize __start { };
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
				TSize __start { };
				TSize __pos { };
				Container __container = Container();
				while (__pos != StringType::npos)
				{
					__pos = str.find(separator, __start);
					__container.push_back(str.substr(__start, __pos - __start + 1));
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
					str = str.erase(str.find(charCode, str.size() - 1), 1);
			}
			
			template<typename ... Strings>
			TString concat(Strings... args)
			{
				TString __result { };
				std::initializer_list<TString> __argList { std::forward<Strings>(args)... };
				for (const auto& value : __argList)
					__result.append(value);
				
				return __result;
			}
			
			template<typename ... Args, typename Char = typename TString::value_type>
			TString buildPath(Char separator, const Args& ... args)
			{
				TString __res { };
				std::initializer_list<TString> __strings { args... };
				TSize __index = 0;
				
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
			
			template<
					class TString,
					typename... TArgs,
					typename TChar = typename TString::value_type
			>
			void buildPath(TString& returnValue, TChar separator, const TArgs& ... args)
			{
				std::list<TString> __strings { args... };
				
				// if (sizeof...(TArgs) > 1)
				// {
					TSize __index = 0;
					for (const auto& str : __strings)
					{
						returnValue.append(str);
						if (++__index != __strings.size())
						{
							returnValue += separator;
						}
					}
				// } else
				// {
				// 	returnValue.append(1, separator).append(__strings.back());
				// }
			}
			
			template<typename String, typename TChar = typename String::value_type>
			void encloseWith(String& returnValue, TChar startChar, TChar endChar)
			{
				returnValue.insert(0, 1, startChar);
				returnValue.insert(returnValue.length(), 1, endChar);
			}
			
			template<typename TIIterator>
			TString toString(TIIterator first, TIIterator last, const TByte* sep = ", ", int width = -1)
			{
				const TSize __distance = std::distance(first, last);
				if (__distance < 1)
					return "[]";
				
				TSize __counter {};
				std::ostringstream __oss{};
				__oss << '[';
				
				for(; first != last; ++first, ++__counter)
				{
					__oss << *first;
					if (__distance != 1 && __counter != __distance - 1)
						__oss << sep;
					if (width > 0 && __counter%width == 0)
						__oss << '\n';
				}
				__oss << ']';
				
				return __oss.str();
			}
			
			template<typename TIIterator>
			TString toString(TIIterator first, TIIterator last, int width)
			{
				return std::move(toString(first, last, ", ", width));
			}
			
			template<typename TSeqContainer>
			inline TString toString(TSeqContainer container, const TByte* sep = ", ", int width = -1)
			{
				return toString(std::begin(container), std::end(container), sep, width);
			}
			
			template<typename TSeqContainer>
			inline TString toString(TSeqContainer container, Int32 width)
			{
				return std::move(toString(std::begin(container), std::end(container), width));
			}
			
			TWString toWstring(const TString& str);
			
			int wstrToInt(const TWString& value);
			
			#ifdef HAVE_CXX17
			
			inline void removePrefix(TStringView& strView, TSize n)
			{
				strView.remove_prefix(n);
			};
			
			inline void removeSuffix(TStringView& strView, TSize n)
			{
				strView.remove_suffix(n);
			};
			
			inline TStringView range(TStringView& stringView, TSize begin, TSize end)
			{
				auto len = std::min(end, stringView.length());
				return TStringView(stringView.data() + begin, len);
			}
			
			template<typename Functor>
			inline TStringView rangefn(TStringView& stringView, TSize begin, TSize end, Functor f)
			{
				auto len = std::min(end, stringView.length());
				auto rng = f(TStringView(stringView.data() + begin, len));
				return rng;
			}
			
			#endif
			
			//TODO: Fix for integral and string types
			#ifdef USE_TO_STRING_CONVERSION
			template<class Map,
				typename KeyType    = typename Map::TKey,
				typename MappedType = typename Map::TMapped>
		TString toString(const Map& map,
						  char pairDelim, char delim = ',')
		{
			TString __result;
			bool keyIsArithmetic{};
			bool MappedIsArithmetic{};
			typedef typename TString::TSizeype TSizeype;
			#if HAVE_CXX17
			typedef std::remove_reference_t<std::remove_const_t<KeyType>>       non_const_key_t;
			typedef std::remove_reference_t<std::remove_const_t<MappedType>>    non_const_mapped_t;
			keyIsArithmetic    = std::is_arithmetic_v<non_const_key_t>;
			MappedIsArithmetic = std::is_arithmetic_v<non_const_mapped_t>;
			#elif HAVE_CXX14 || HAVE_CXX11
			typedef std::remove_reference_t<std::remove_const_t<KeyType>>       non_const_key_t;
			typedef std::remove_reference_t<std::remove_const_t<MappedType>>    non_const_mapped_t;
			keyIsArithmetic    = std::is_arithmetic<non_const_key_t>::m_value;
			MappedIsArithmetic = std::is_arithmetic<non_const_mapped_t>::m_value;
			#elif defined(HAVE_LEGACY)
			typedef typename ArithmeticType non_const_rvalue_type;
			#endif

			TSizeype __index {};
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
			TString to_string(ArithmeticType value)
			{
				#ifdef HAVE_CXX17
				typedef std::remove_reference_t<std::remove_const_t<ArithmeticType>> non_const_rvalue_t;
				constexpr bool __isSame = std::is_same_v<TString, ArithmeticType>;
				#elif defined(HAVE_CXX14) or defined(HAVE_CXX11)
				typedef typename std::remove_reference<typename std::remove_const<ArithmeticType>::type>::type non_const_rvalue_t;
				constexpr bool __isSame = std::is_same<TString, ArithmeticType>::m_value;
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
			
			int uStringToInt(const TUString& value);
			
			TUString intToUstring(Int32 value);
		} // namespace Strings
	} // namespace Utilities
} // namespace Dixter