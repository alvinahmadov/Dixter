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
#include "Exception.hpp"
#include "Types.hpp"

#ifdef HAVE_CXX17

#include <optional>

#else
#include <boost/optional.hpp>
#endif


namespace Dixter
{
	namespace OpenTranslate
	{
		#ifdef HAVE_CXX17
		template<typename T>
		using TOptional = std::optional<T>;
		
		class TToken;
		
		class TTokenizer;
		
		std::ostream& operator<<(std::ostream& out, const std::vector<TToken*>& pTokens);
		
		std::ostream& operator<<(std::ostream& out, const TTokenizer* tokenizer);
		
		#else
		template <typename T>
		using TOptional = T;
		#endif
		
		enum class TokenMode
		{
			kNone,
			kComplex,
			kMultiLine,
			kSimple
		};
		
		TString tokenModeToString(TokenMode mode);
		
		class TToken : public CopyConstructible
		{
		public:
			struct TokenInfo : public CopyConstructible
			{
				UInt32 wordCount = 0;
				UInt32 lines = 0;
				UInt32 punctuationChars = 0;
				std::vector<Int32> punctPositions;
			};
		
		private:
			
			template<typename T>
			struct TTokenHolder : public CopyConstructible
			{
				TTokenHolder() noexcept = default;
				
				~TTokenHolder() noexcept = default;
				
				std::vector<T> tokens;
				
				TokenInfo tokenInfo;
			};
		
		public:
			#ifdef HAVE_CXX17
			using TValue            = TStringView; //TStringView;
			using TConstValue       = TValue;
			#else
			using TValue            = TString;
			using TConstValue       = const TValue&;
			#endif
			using TTokenVector      = std::vector<TValue>;
			using TTokenValueHolder = TTokenHolder<TValue>;
		
		public:
			TToken() = delete;
			
			explicit TToken(TConstValue part) noexcept;
			
			explicit TToken(const TToken& o) noexcept;
			
			~TToken() noexcept;
			
			bool push(TConstValue chunk, const TokenInfo& info);
			
			bool isEmpty() const;
			
			size_t size() const;
			
			TOptional<TTokenVector>
			getChunks() const;
			
			TConstValue getChunk(size_t index) const;
			
			TConstValue getPart() const;
			
			const TokenInfo& getInfo() const;
			
			TokenInfo& getInfo();
			
			void setInfo(const TokenInfo& info);
		
		private:
			TConstValue m_token;
			
			TTokenValueHolder* m_chunks;
		};
		
		std::ostream& operator<<(std::ostream& out, const TToken::TokenInfo& info);
		
		/** @class Tokenizer
		 * @brief Tokenizer is a class which
		 * */
		class TTokenizer
		{
			// friend std::ostream& operator<<(std::ostream& out, const TTokenizer* tokenizer);
			using TValue        = TToken::TValue;
			using TConstValue   = TToken::TConstValue;
			using TTokenVector  = TToken::TTokenVector;
		public:
			TTokenizer();
			
			~TTokenizer();
			
			/** @brief
			 *
			 * */
			void tokenize(TConstValue sentence);
			
			/** @brief A sentence is multiline when it has more than one complex or/and simple parts and
			 * each separated by terminating dots
			 * */
			bool isMultiline(TConstValue token, UInt32& lineCount) const;
			
			/** @brief Test and count a complex parts of sentence.
			 * A complex part is when one sentence is separated by different meanings
			 * @returns A number of complex parts
			 * */
			bool isComplex(TConstValue token, UInt32& complexCount) const;
			
			static UInt32 countWords(TConstValue token);
			
			const std::vector<TToken*>&
			getTokens() const;
			
			TString
			toString() const;
		
		protected:
			/** @addtogroup mutating
			 *  @brief Divide a multiline token to small parts
			 *  @param tokens A token to analyse. After operation token
			 * */
			bool tokenizeLines(TConstValue tokens);
			
			bool tokenizeComplex(TConstValue token);
			
			bool tokenizeWords(TConstValue token);
			
			void readToken(TConstValue token,
			               char delim, const Int64& maxCount,
			               std::vector<TValue*>& tokens);
			
			static void sanitizeExcluded(TValue& token);
			
			static void sanitizeExcluded(TValue&& token);
			
			static void sanitize(TValue& token, const int* charList = nullptr);
			
			/** @brief Test for containing an char element in token
			 * @param token A token to be tested
			 * @param point A char point to count for
			 * @returns Number of char points in the token
			 * */
			static UInt32 contains(TConstValue token,
			                       std::function<bool(TValue::const_reference)>&& predicate);
			
			static bool empty(TValue::const_iterator value);
		
		private:
			TokenMode m_mode;
			
			TValue m_token;
			
			std::vector<TToken*> m_tokens;
		};
	}
}