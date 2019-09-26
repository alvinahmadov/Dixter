/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include <algorithm>
#include <sstream>
#include <list>

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
		template<
				typename TClass,
				typename FFunctor
		>
		TClass& compareAssign(TClass* lhs, const TClass& rhs, FFunctor&& functor)
		{
			if (lhs != std::addressof(rhs))
				functor();
			
			return *lhs;
		}
		
		namespace Algorithms
		{
			template<
					typename TContainer,
					typename FFunctor
			>
			inline FFunctor&
			forEach(TContainer& container, FFunctor&& functor)
			{
				return std::for_each(std::begin(container), std::end(container), functor);
			};
			
			template<
					typename TContainer,
					typename FFunctor
			>
			inline FFunctor&
			forEach(const TContainer& container, FFunctor&& functor)
			{
				return std::for_each(std::begin(container), std::end(container), functor);
			};
			
			template<
					typename TContainer,
					typename FFunctor
			>
			inline FFunctor&
			forEach(TContainer* container, FFunctor&& functor)
			{
				return std::for_each(std::begin(*container), std::end(*container), functor);
			};
			
			template<
					typename TNumber,
					typename FFunctor
			>
			inline void
			forEach(TNumber& start, const TNumber& end, FFunctor&& functor)
			{
				for (start = 0; start < end; start++)
					functor(start);
			};
			
			/// Full copy wrapper for std::copy
			template<
					typename TContainer,
					typename TIterator = typename TContainer::iterator
			>
			inline TIterator
			copy(const TContainer& input, TContainer& output)
			{
				if (not std::empty(output))
					output.clear();
				
				return std::copy(std::cbegin(input), std::cend(input), std::begin(output));
			}
			
			/// Full copy pointer template specialization
			template<
					typename TContainer,
					typename TIterator = typename TContainer::iterator
			>
			inline TIterator
			copy(const TContainer* input, TContainer* output)
			{
				return copy(*input, *output);
			}
			
			/// Full move wrapper for std::move
			template<
					typename TContainer,
					typename TIterator = typename TContainer::iterator
			>
			inline TIterator
			move(const TContainer& input, TContainer& output)
			{
				if (not std::empty(output))
					output.clear();
				
				return std::move(std::cbegin(input), std::cend(input), std::begin(output));
			}
			
			/// Full move pointer template specialization
			template<
					typename TContainer,
					typename TIterator = typename TContainer::iterator
			>
			inline TIterator
			move(const TContainer* input, TContainer* output)
			{
				return move(*input, *output);
			}
			
			template<typename TContainer>
			inline bool __f_ofSameSize(TContainer& containerA, TContainer& containerB)
			{
				return static_cast<TSize>(std::size(containerA)) ==
						static_cast<TSize>(std::size(containerB));
			}
			
			template<
					typename TContainerA,
					typename TContainerB
			>
			inline bool __f_ofSameSize(TContainerA& containerA, TContainerB& containerB)
			{
				return static_cast<TSize>(std::size(containerA)) ==
						static_cast<TSize>(std::size(containerB));
			}
			
			template<
					typename TContainer,
					typename FFunctor,
					typename TIterator = typename TContainer::iterator
			>
			void foreachCompound(TContainer& containerA, TContainer& containerB, FFunctor&& functor)
			{
				if(not __f_ofSameSize(containerA, containerB))
					return;
				
				TIterator __lFirst = std::begin(containerA), __lLast = std::end(containerA);
				TIterator __rFirst = std::begin(containerB), __rLast = std::end(containerB);
				
				for (; __lFirst != __lLast and __rFirst != __rLast;
					   ++__rFirst, ++__lFirst)
				{
					functor(*__lFirst, *__rFirst);
				}
			}
			
			template<
					typename TContainerA,
					typename TContainerB,
					typename FFunctor,
					typename TInputIterator = typename TContainerA::const_iterator,
					typename TOutputIterator = typename TContainerB::iterator
			>
			void foreachCompound(TContainerA& containerA, TContainerB& containerB, FFunctor&& functor)
			{
				if(not __f_ofSameSize(containerA, containerB))
					return;
				
				TInputIterator  __lFirst = std::cbegin(containerA), __lLast = std::cend(containerA);
				TOutputIterator __rFirst = std::begin(containerB),  __rLast = std::end(containerB);
				
				for (; __lFirst != __lLast and __rFirst != __rLast;
					   ++__rFirst, ++__lFirst)
				{
					functor(*__lFirst, *__rFirst);
				}
			}
			
			template<
					typename TContainer,
					typename FFunctor,
					typename TIterator = typename TContainer::iterator
			>
			void iterateParallel(TContainer& containerA, TContainer& containerB, FFunctor&& functor)
			{
				if(not __f_ofSameSize(containerA, containerB))
					return;
				
				TIterator __lFirst = std::begin(containerA), __lLast = std::begin(containerA);
				TIterator __rFirst = std::begin(containerB), __rLast = std::begin(containerB);
				
				for (; __lFirst != __lLast and __rFirst != __rLast;
					   ++__rFirst, ++__lFirst)
				{
					functor(__lFirst, __rFirst);
				}
			}
			
			template<
					typename TContainer,
					typename FFunctor
			>
			inline void iterateParallel(TContainer* containerA, TContainer* containerB, FFunctor&& functor)
			{
				iterateParallel(*containerA, *containerB, std::forward<FFunctor>(functor));
			}
			
			template<
					typename TContainer,
					typename TIterator,
					typename TDifference = typename std::iterator_traits<TIterator>::difference_type
			>
			TDifference
			count(TIterator first, TIterator last, const TContainer& container)
			{
				TDifference __diff {};
				
				for (const auto& __v : container)
					__diff += std::count(first, last, __v);
				
				return __diff;
			}
			
		} // namespace Algorithms
		
		namespace Strings
		{
			/**
			 * \brief Create string representation of data in container separated by explodeBy
			 * \tparam TSeqContainer The type of sequence container from which generated data representation
			 * \tparam TString The type of the string, unicode or not
			 * \tparam TChar The type of the char by which data exploded
			 * \param container Container holding data values
			 * \param separator Character by which elements separated in representation
			 * \return string representation
			 * */
			template<
					typename TSeqContainer,
					typename TString = typename TSeqContainer::value_type,
					typename TChar = typename TString::value_type
			>
			TString
			join(const TSeqContainer& container, TChar separator = TChar(','))
			{
				TString __result {};
				TSize __index {}, __start {};
				
				for (const TString& __val : container)
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
			
			/** \brief Generate a container from string representation str of data separated by delimiter
			  * \tparam TSeqContainer The type of container from which generated data representation
			  * \tparam TString The type of the string, unicode or not
			  * \tparam TChar The type of the char by which data exploded
			  * \param str String which holds separate values
			  * \param separator Character by which the data in string is sepearated
			  * \return Newly generated container (list, vector, stack)
			  */
			template<
					typename TSeqContainer,
					typename TString,
					typename TChar = typename TString::value_type
			>
			TSeqContainer
			split(const TString& str, TChar separator = TChar(','))
			{
				TSize __start {}, __pos {};
				TSeqContainer __container {};
				
				while (__pos != TString::npos)
				{
					__pos = str.find(separator, __start);
					__container.push_back(str.substr(__start, __pos - __start + 1));
					__start = __pos + 1;
				}
				
				return __container;
			}
			
			template<
					typename TString,
					typename TChar = typename TString::value_type
			>
			void trim(TString& str, const TChar charCode = ' ', bool symmetrical = true)
			{
				str = str.erase(str.find(charCode), 1);
				
				if (symmetrical)
					str = str.erase(str.find(charCode, str.size() - 1), 1);
			}
			
			template<typename... TStringArgs>
			TString
			concat(TStringArgs&&... strings)
			{
				TString __result {};
				std::initializer_list<TString> __argList { std::forward<TStringArgs>(strings)... };
				
				for (const auto& value : __argList)
					__result.append(value);
				
				return __result;
			}
			
			template<
					typename... TArgs,
					typename TChar = typename TString::value_type
			>
			TString
			buildPath(TChar separator, const TArgs&... args)
			{
				TString __res {};
				std::initializer_list<TString> __strings { args... };
				TSize __index(0);
				
				if (sizeof...(TArgs) > 1)
				{
					for (const auto& str : __strings)
					{
						__res.append(str);
						if (++__index != __strings.size())
							__res += separator;
					}
				}
				else
				{
					__res += separator;
					__res += *__strings.end();
				}
				
				return __res;
			}
			
			template<
					typename TString,
					typename... TArgs,
					typename TChar = typename TString::value_type
			>
			void buildPath(TString& returnValue, TChar separator, const TArgs&... args)
			{
				std::initializer_list<TString> __strings { args... };
				TSize __index(0);
				
				for (const auto& str : __strings)
				{
					returnValue.append(str);
					
					if (++__index != __strings.size())
						returnValue += separator;
				}
			}
			
			template<
					typename TReturnString,
					typename TChar = typename TReturnString::value_type
			>
			void encloseWith(TReturnString& returnValue, TChar startChar, TChar endChar)
			{
				returnValue.insert(0, 1, startChar);
				returnValue.insert(returnValue.length(), 1, endChar);
			}
			
			template<typename TIterator>
			TString
			toString(TIterator first, TIterator last, const TByte* sep = ", ", int width = -1)
			{
				const TSize __distance = std::distance(first, last);
				
				if (__distance < 1)
					return "[]";
				
				TSize __counter {};
				std::ostringstream __oss {};
				
				__oss << '[';
				for (; first != last;
					   ++first, ++__counter)
				{
					__oss << *first;
					if (__distance != 1 and __counter != __distance - 1)
						__oss << sep;
					if (width > 0 and __counter % width == 0)
						__oss << '\n';
				}
				__oss << ']';
				
				return __oss.str();
			}
			
			template<typename TIIterator>
			inline TString
			toString(TIIterator first, TIIterator last, int width)
			{
				return std::move(toString(first, last, ", ", width));
			}
			
			template<typename TSeqContainer>
			inline TString
			toString(TSeqContainer container, const TByte* sep = ", ", int width = -1)
			{
				return toString(std::begin(container), std::end(container), sep, width);
			}
			
			template<typename TSeqContainer>
			inline TString
			toString(TSeqContainer container, Int32 width)
			{
				return std::move(toString(std::begin(container), std::end(container), width));
			}
			
			template<
					typename TNumber,
					typename TString
			>
			TString 
			toString(TNumber value)
			{
				#ifdef HAVE_CXX17
				static_assert(std::is_arithmetic_v<TNumber>);
				using TRValue = std::remove_reference_t<std::remove_const_t<TNumber>>;
				auto __nonConstRvalue = static_cast<TRValue>(value);
				
				if constexpr (std::is_same_v<TString, TNumber>)
					return __nonConstRvalue;
				else
					return std::to_string(__nonConstRvalue);
				#elif defined(HAVE_CXX14) or defined(HAVE_CXX11)
				static_assert(std::is_arithmetic<TNumber>::value);
				typedef typename std::remove_reference<typename std::remove_const<TNumber>::type>::type non_const_rvalue_t;
				const bool __isSame = std::is_same<TString, TNumber>::value;
				if (__isSame)
					return __nonConstRvalue;
				else
					return std::to_string(__nonConstRvalue);
				#endif
				
				return TString();
			}
			
			#ifdef HAVE_CXX17
			
			inline void
			removePrefix(TStringView& strView, TSize n)
			{
				strView.remove_prefix(n);
			};
			
			inline void
			removeSuffix(TStringView& strView, TSize n)
			{
				strView.remove_suffix(n);
			};
			
			inline TStringView
			range(TStringView& stringView, TSize begin, TSize end)
			{
				auto __len = std::min(end, stringView.length());
				
				return TStringView(stringView.data() + begin, __len);
			}
			
			template<typename FFunctor>
			inline TStringView
			rangefn(TStringView& stringView, TSize begin, TSize end, FFunctor&& functor)
			{
				auto __len = std::min(end, stringView.length());
				auto __range = functor(TStringView(stringView.data() + begin, __len));
				
				return __range;
			}
			
			#endif // HAVE_CXX17
			
			template<
					typename TResultString,
					typename TArgString
			>
			inline TResultString
			convertString(const TArgString& argString)
			{
				return TResultString(argString.data());
			}
			
		} // namespace Strings
	} // namespace Utilities
} // namespace Dixter