/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include <vector>
#include <set>
#include "Exception.hpp"
#include "Types.hpp"
#include <sstream>

#ifdef HAVE_CXX17

#include <optional>
#include <Utilities.hpp>

#else
#include <boost/optional.hpp>
#endif


namespace Dixter
{
	namespace OpenTranslate
	{
		class TToken : public CopyConstructible
		{
		public:
			struct TTokenInfo final : public CopyConstructible
			{
				TTokenInfo() = default;
				
				inline TString toString() const
				{
					std::ostringstream __oss;
					__oss << "Words: " << wordCount
					      << ", Punctuation chars: " << punctuationChars
					      << ", is complex: " << ( isComplex ? "yes" : "no" )
					      << ", punct. pos: " << Utilities::Strings::toString(punctPositions);
					return __oss.str();
				}
				
				bool isComplex;
				UInt32 wordCount;
				UInt32 punctuationChars;
				std::vector<Int32> punctPositions;
				std::unordered_map<Int32, TByte> punctuations;
			};
		
		public:
			#ifdef HAVE_CXX17
			using TValue            = TStringView;
			using TConstValue       = TValue;
			#else
			using TValue            = TString;
			using TConstValue       = const TValue&;
			#endif
			using TTokenValueHolder = std::vector<TValue>;
		
		public:
			TToken() noexcept;
			
			explicit TToken(const TToken& o) noexcept;
			
			~TToken() noexcept = default;
			
			bool push(TConstValue chunk, const TTokenInfo& info);
			
			bool isEmpty() const;
			
			TSize size() const;
			
			const TTokenValueHolder&
			getChunks() const;
			
			TConstValue getChunk(TSize index) const;
			
			const TTokenInfo& getInfo() const;
			
			TTokenInfo& getInfo();
			
			void setInfo(const TTokenInfo& info);
			
			inline auto begin()
			{
				return m_chunks.begin();
			}
			
			inline auto end()
			{
				return m_chunks.end();
			}
			
			inline const auto cbegin() const
			{
				return m_chunks.cbegin();
			}
			
			inline const auto cend() const
			{
				return m_chunks.cend();
			}
		
		private:
			TTokenValueHolder m_chunks;
			TTokenInfo m_info;
		};
		
		std::ostream& operator<<(std::ostream& out, const TToken::TTokenInfo& info);
		
		/** @class Tokenizer
		 * @brief Tokenizer is a class which
		 * */
		class TTokenizer
		{
			using TValue             = TToken::TValue;
			using TValueVector       = std::vector<TValue*>&;
			using TConstValue        = TToken::TConstValue;
			using TTokenValueHolder  = TToken::TTokenValueHolder;
		public:
			TTokenizer(TConstValue sentence = "") noexcept;
			
			~TTokenizer() noexcept;
			
			/** @brief
			 *
			 * */
			void tokenize(TConstValue sentence);
			
			const TTokenValueHolder&
			getTokens() const;
			
			static UInt32 countWords(TConstValue token);
			
			TString toString() const;
		
		protected:
			
			static void readToken(TConstValue token, const Int64& maxCount,
			                      TByte sep, TTokenValueHolder& chunks);
			
			static TValue& cleanExcluded(TValue& token, TSize begin, TSize end);
			
			static TValue& cleanJunk(TValue& token, const int* charList = nullptr);
			
			/** @brief Test for containing an char element in token
			 * @param token A token to be tested
			 * @param point A char point to count for
			 * @returns Number of char points in the token
			 * */
			static UInt32 contains(TConstValue token,
			                       std::function<bool(TValue::const_reference)>&& predicate);
			
			static bool empty(TValue::const_iterator value);
		
		public:
			static const TByte s_delimiter;
			static const TByte s_separator;
		
		private:
			TToken* m_token;
		};
	}
}