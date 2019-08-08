/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
 
#pragma once


#include <list>
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <StringView.hpp>

#include "Commons.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		class TokenData;
		
		class Tokenizer;
		
		struct TokenInfo;
		
		std::ostream& operator<<(std::ostream& out, const std::vector<TokenData*>* pTokens);
		
		std::ostream& operator<<(std::ostream& out, const TokenInfo& info);
		
		std::ostream& operator<<(std::ostream& out, const Tokenizer* tokenizer);
		
		string_view_t range(const string_view_t& stringView, size_t begin, size_t end);
		
		enum class TokenMode
		{
			kNone,
			kComplex,
			kMultiLine,
			kSimple
		};
		
		string_t tokenModeToString(TokenMode mode);
		
		struct TokenInfo : public CopyConstructible
		{
			ui32 wordCount = 0;
			ui32 lines = 0;
			ui32 punctuationChars = 0;
			std::vector<i32> punctPositions;
		};
		
		template<typename T>
		struct Token
		{
			using pointer_t = T*;
			using ref_t = T&;
			
			Token()
					: tokens { new std::list<pointer_t>() },
					  tokenInfo { }
			{ }
			
			~Token()
			{
				SAFE_RELEASE(tokens)
			}
			
			std::list<pointer_t>* tokens;
			
			TokenInfo tokenInfo;
		};
		
		class TokenData
		{
		public:
			using value_type = string_view_t;
			using token_type = Token<value_type>::pointer_t;
		
		public:
			TokenData();
			
			explicit TokenData(value_type* part);
			
			~TokenData();
			
			bool push(const value_type& chunk, const TokenInfo& info);
			
			bool isEmpty() const;
			
			size_t size() const;
			
			const std::list<token_type>* getChunks() const;
			
			std::list<token_type>* getChunks();
			
			const value_type& getChunk(size_t index) const;
			
			value_type& getChunk(size_t index);
			
			const value_type* getPart() const;
			
			value_type* getPart();
			
			const TokenInfo& getInfo() const;
			
			TokenInfo& getInfo();
			
			void setInfo(const TokenInfo& info);
		
		protected:
			value_type* m_token;
			
			Token<value_type>* m_chunks;
		};
		
		/** @class Tokenizer
		 * @brief Tokenizer is a class which
		 * */
		class Tokenizer
		{
			friend std::ostream& operator<<(std::ostream& out, const Tokenizer* tokenizer);
		
		public:
			Tokenizer();
			
			~Tokenizer();
			
			/** @brief
			 *
			 * */
			void tokenize(const string_t& sentence);
			
			/** @brief A sentence is multiline when it has more than one complex or/and simple parts and
			 * each separated by terminating dots
			 * */
			bool isMultiline(const string_view_t* token, ui32& lineCount);
			
			/** @brief Test and count a complex parts of sentence.
			 * A complex part is when one sentence is separated by different meanings
			 * @returns A number of complex parts
			 * */
			bool isComplex(const string_view_t* token, ui32& complexCount);
			
			static ui32 countWords(const string_view_t* token);
			
			const std::vector<TokenData*>* getTokenData() const;
		
		protected:
			/** @addtogroup mutating
			 *  @brief Divide a multiline token to small parts
			 *  @param tokens A token to analyse. After operation token
			 * */
			bool tokenizeLines(const string_view_t* tokens);
			
			bool tokenizeComplex(const string_view_t* token);
			
			bool tokenizeWords(const string_view_t* token);
			
			void readToken(const string_view_t* token,
			               char delim, const i64& maxCount,
			               std::vector<string_view_t*>& tokens);
			
			static string_view_t& trimExcluded(string_view_t& token);
			
			static string_view_t& trimExcluded(string_view_t&& token);
			
			static string_view_t& trim(string_view_t& token, const int* charList = nullptr);
			
			/** @brief Test for containing an char element in token
			 * @param token A token to be tested
			 * @param point A char point to count for
			 * @returns Number of char points in the token
			 * */
			static ui32 contains(const string_view_t* token, std::function<bool(string_view_t::reference)>&& predicate);
			
			static bool empty(string_view_t::const_pointer value);
		
		private:
			TokenMode m_mode;
			
			string_view_t* m_token;
			
			std::vector<TokenData*>* m_tokenData;
		};
	}
}