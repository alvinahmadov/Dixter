/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <algorithm>
#include <sstream>
#include "Tokenizer.hpp"
#include "Exception.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"
#include "Commons.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		const Int32 nonAllowedCharsSize = 6;
		
		const Int32 nonAllowedChars[nonAllowedCharsSize] {
				9,
				10, //newline
				32, //whitespace
				44, // (
				46, // )
				-1  //EOF
		};
		
		namespace AlgoUtils   = Utilities::Algorithms;
		namespace StringUtils = Utilities::Strings;
		
		#ifdef HAVE_CXX17
		
		inline void removePrefix(TStringView& strView, TSize n)
		{
			strView.remove_prefix(n);
		};
		
		inline void removeSuffix(TStringView& strView, TSize n)
		{
			strView.remove_suffix(n);
		};
		
		#else
		
		inline void removePrefix(TString& str, TSize n)
		{
			dxUNUSED(str)
			dxUNUSED(n)
		};
		
		inline void removeSuffix(TString& str, TSize n)
		{
			dxUNUSED(str)
			dxUNUSED(n)
		};
		
		inline TString
		range(const TString& string, TSize begin, TSize end)
		{
			auto len = std::min(end, string.length());
			return TString{string.data() + begin, len};
		}
		
		#endif
		
		bool isWhiteSpace(const TByte& c)
		{
			return c == g_whiteSpace;
		}
		
		///TokenData Impl.
		TToken::TToken() noexcept
				: m_chunks(TTokenValueHolder()),
				  m_info(TTokenInfo())
		{ }
		
		TToken::TToken(const TToken& o) noexcept
				: CopyConstructible(o),
				  m_chunks(o.m_chunks)
		{ }
		
		bool TToken::push(TToken::TConstValue chunk, const TToken::TTokenInfo& info)
		{
			if (not chunk.empty())
			{
				m_chunks.push_back(chunk);
				m_info = info;
				return true;
			}
			return false;
		}
		
		inline bool TToken::isEmpty() const
		{
			return m_chunks.empty();
		}
		
		inline TSize TToken::size() const
		{
			return m_chunks.size();
		}
		
		const TToken::TTokenValueHolder&
		TToken::getChunks() const
		{
			return m_chunks;
		}
		
		TToken::TConstValue
		TToken::getChunk(TSize index) const
		{
			if (m_chunks.size() == 1)
				return m_chunks.back();
			
			if (m_chunks.size() > 1)
				return m_chunks.at(index);
			
			throw NotFoundException("%s:%d View by index %d not found", __FILE__, __LINE__, index);
		}
		
		const TToken::TTokenInfo&
		TToken::getInfo() const
		{
			return m_info;
		}
		
		TToken::TTokenInfo&
		TToken::getInfo()
		{
			return m_info;
		}
		
		void TToken::setInfo(const TToken::TTokenInfo& info)
		{
			m_info = info;
		}
		
		///Tokenizer Impl.
		const TByte TTokenizer::s_delimiter { g_whiteSpace };
		
		const TByte TTokenizer::s_separator { g_comma };
		
		TTokenizer::TTokenizer(TTokenizer::TConstValue sentence) noexcept
				: m_token(new TToken())
		{
			if (sentence.length() > 0)
				this->tokenize(sentence);
		}
		
		TTokenizer::~TTokenizer() noexcept
		{
			delete m_token;
		}
		
		void TTokenizer::tokenize(TTokenizer::TConstValue sentence)
		{
			TToken::TTokenValueHolder __chunks;
			TToken::TTokenInfo __info;
			
			const UInt32 __wordCount = countWords(sentence);
			__info.wordCount = __wordCount;
			__info.punctuationChars = std::count(sentence.cbegin(), sentence.cend(), s_separator);
			__info.punctuations.try_emplace(
					std::count(sentence.cbegin(), sentence.cend(), s_separator),
					s_separator);
			__info.isComplex = AlgoUtils::count<std::initializer_list<TByte>>
					(sentence.cbegin(), sentence.cend(), { ',', ':', '\"' }) > 0;
			
			this->readToken(sentence, __wordCount + 1, g_whiteSpace, __chunks);
			
			for (const auto& __chunk : __chunks)
				m_token->push(__chunk, __info);
		}
		
		const TToken::TTokenValueHolder&
		TTokenizer::getTokens() const
		{
			return m_token->getChunks();
		}
		
		UInt32
		TTokenizer::countWords(TTokenizer::TConstValue token)
		{
			UInt32 __count {};
			for (const auto& __c : token)
				if (isWhiteSpace(__c))
					++__count;
			return __count;
		}
		
		TString
		TTokenizer::toString() const
		{
			std::ostringstream __oss;
			auto __chunks = m_token->getChunks();
			
			if (not __chunks.empty())
				for (const auto& __chunk : __chunks)
					__oss << __chunk << std::endl;
			
			return __oss.str();
		};
		
		/*
		bool TTokenizer::tokenizeComplex(TTokenizer::TConstValue token)
		{
			UInt32 parts { };
			if (not isComplex(token, parts))
				return false;
			
			std::vector<TValue> __tokens { };
			TToken::TTokenInfo __info { };
			// if (m_token.empty())
			// 	m_token.push_back(new TToken());
			
			__info = m_token.back()->getInfo();
			__info.punctuationChars = parts;
			
			m_token.back()->setInfo(__info);
			tokenizeWords(token);
			
			return true;
		}
		*/
		
		void TTokenizer::readToken(TTokenizer::TConstValue token, const Int64& maxCount,
								   TByte sep, TTokenizer::TTokenValueHolder& chunks)
		{
			if (token.empty() or not maxCount)
			{
				printl_log("Token is empty")
				return;
			}
			
			TSize __pos {}, __end {};
			TValue __chunk;
			while (( __end = token.find_first_of(sep, __pos)) <= token.length())
			{
				__chunk = cleanExcluded(token, __pos, __end - __pos);
				chunks.push_back(__chunk);
				__pos = std::min(__end + 1, token.length());
			}
			__chunk = cleanExcluded(token, __pos, token.length() - __pos - 1);
			chunks.push_back(__chunk);
		}
		
		inline TTokenizer::TValue&
		TTokenizer::cleanExcluded(TTokenizer::TValue& token, TSize begin, TSize end)
		{
			auto __range = StringUtils::range(token, begin, end);
			return cleanJunk(__range, nonAllowedChars);
		}
		
		TTokenizer::TValue&
		TTokenizer::cleanJunk(TTokenizer::TValue& token, const int* charList)
		{
			if (not charList)
				return;
			
			Int32 index = 0;
			Int32 chr = 0;
			while (( chr = charList[index++] ) != -1)
			{
				removePrefix(token, std::min(token.find_first_not_of(chr), token.size()));
				removeSuffix(token, token.size() - std::min(token.find(chr), token.size()));
			}
			
			return token;
		}
		
		inline UInt32
		TTokenizer::contains(TTokenizer::TConstValue token,
							 std::function<bool(TTokenizer::TValue::const_reference)>&& predicate)
		{
			return static_cast<UInt32>(std::count_if(token.cbegin(), token.cend(), predicate));
		}
		
		bool TTokenizer::empty(TTokenizer::TValue::const_iterator token)
		{
			int index = 0;
			Int32 ch = 0;
			while (( ch = token[index] ) != '\0')
			{
				if (not std::isalnum(ch))
					return false;
				
				++index;
			}
			return true;
		}
	}
}
