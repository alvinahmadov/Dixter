/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <algorithm>

#include "Tokenizer.hpp"


namespace Dixter
{
	namespace OpenTranslate
	{
		const int nonAllowedCharsSize = 6;
		
		const int nonAllowedChars[nonAllowedCharsSize] {
				9,
				10, //newline
				32, //whitespace
				44, // (
				46, // )
				-1  //EOF
		};
		
		///TokenData Impl.
		std::ostream& operator<<(std::ostream& out, const std::vector<TokenData*>* pTokens)
		{
			for (const auto& tokenData : *pTokens)
			{
				if (tokenData->getChunks() != nullptr)
				{
					printl_log(tokenData->getInfo())
					for (const auto& token : *tokenData->getChunks())
					{
						out << *token;
						out << std::endl;
					}
				}
			}
			return out;
		}
		
		std::ostream& operator<<(std::ostream& out, const TokenInfo& info)
		{
			out << "Lines: " << info.lines
			    << ", Word Count: " << info.wordCount
			    << ", Punctuations: " << info.punctuationChars
			    << ", Positions: ";
			for (const auto& position : info.punctPositions)
			{
				out << position;
			}
			out << std::endl;
			return out;
		}
		
		std::ostream& operator<<(std::ostream& out, const Tokenizer* tokenizer)
		{
			out << tokenizer->m_tokenData;
			return out;
		}
		
		string_view_t range(const string_view_t& stringView, size_t begin, size_t end)
		{
			auto len = std::min(end, stringView.length());
			return string_view_t(stringView.data() + begin, len);
		}
		
		string_t tokenModeToString(TokenMode mode)
		{
			string_t ret { };
			switch (mode)
			{
				case TokenMode::kSimple : ret = xSTR(kSimple);
					break;
				case TokenMode::kMultiLine : ret = xSTR(kMultiLine);
					break;
				case TokenMode::kComplex : ret = xSTR(kComplex);
					break;
				default: ret = xSTR(kNone);
					break;
			}
			return ret;
		}
		
		TokenData::TokenData()
				: m_token { nullptr },
				  m_chunks { new Token<TokenData::value_type> }
		{
			m_chunks->tokens = new std::list<token_type> { };
			m_chunks->tokenInfo = TokenInfo();
		}
		
		TokenData::TokenData(value_type* part)
				: m_token { part },
				  m_chunks { new Token<TokenData::value_type> }
		{
			m_chunks->tokens = new std::list<token_type> { };
			m_chunks->tokenInfo = TokenInfo();
		}
		
		TokenData::~TokenData()
		{
			SAFE_RELEASE(m_chunks->tokens)
			SAFE_RELEASE(m_chunks)
			SAFE_RELEASE(m_token)
		}
		
		bool TokenData::push(const TokenData::value_type& chunk, const TokenInfo& info)
		{
			if (!chunk.empty())
			{
				m_chunks->tokens->push_back(const_cast<token_type>(&chunk));
				m_chunks->tokenInfo = info;
				return true;
			} else
			{
				return false;
			}
		}
		
		inline bool TokenData::isEmpty() const
		{
			return m_chunks->tokens->empty();
		}
		
		inline size_t TokenData::size() const
		{
			return m_chunks->tokens->size();
		}
		
		const std::list<TokenData::token_type>*
		TokenData::getChunks() const
		{
			if (isEmpty())
			{
				return nullptr;
			}
			return m_chunks->tokens;
		}
		
		std::list<TokenData::token_type>*
		TokenData::getChunks()
		{
			if (isEmpty())
			{
				return nullptr;
			}
			return m_chunks->tokens;
		}
		
		const TokenData::value_type&
		TokenData::getChunk(size_t index) const
		{
			if (m_chunks->tokens->size() == 1)
			{
				return *m_chunks->tokens->front();
			}
			if (index > m_chunks->tokens->size())
			{
				throw IllegalArgumentException { "%s:%d Index %d > size: %d.", __FILE__, __LINE__, index, m_chunks->tokens->size() };
			}
			if (m_chunks->tokens->size() > 1)
			{
				size_t __index { };
				for (const auto& sv : *m_chunks->tokens)
				{
					if (__index++ == index)
					{
						return *sv;
					}
				}
			}
			throw NotFoundException("%s:%d View by index %d not found", __FILE__, __LINE__, index);
		}
		
		TokenData::value_type&
		TokenData::getChunk(size_t index)
		{
			if (m_chunks->tokens->size() == 1)
			{
				return *m_chunks->tokens->front();
			}
			if (index > m_chunks->tokens->size())
			{
				throw IllegalArgumentException { "%s:%d Index %d > size: %d.", __FILE__, __LINE__, index, m_chunks->tokens->size() };
			}
			if (m_chunks->tokens->size() > 1)
			{
				size_t __index { };
				for (const auto& sv : *m_chunks->tokens)
				{
					if (__index++ == index)
					{
						return *sv;
					}
				}
			}
			throw NotFoundException("%s:%d View by index %d not found", __FILE__, __LINE__, index);
		}
		
		const TokenData::value_type*
		TokenData::getPart() const
		{
			return m_token;
		}
		
		TokenData::value_type*
		TokenData::getPart()
		{
			return m_token;
		}
		
		inline const TokenInfo&
		TokenData::getInfo() const
		{
			return m_chunks->tokenInfo;
		}
		
		inline TokenInfo&
		TokenData::getInfo()
		{
			return m_chunks->tokenInfo;
		}
		
		void TokenData::setInfo(const TokenInfo& info)
		{
			m_chunks->tokenInfo = info;
		}
		
		///Tokenizer Impl.
		Tokenizer::Tokenizer()
				: m_mode { TokenMode::kNone },
				  m_token { nullptr },
				  m_tokenData { new std::vector<TokenData*>() }
		{ }
		
		Tokenizer::~Tokenizer()
		{
			for (auto& t : *m_tokenData)
				SAFE_RELEASE(t)
			SAFE_RELEASE(m_tokenData)
			SAFE_RELEASE(m_token)
		}
		
		void Tokenizer::tokenize(const string_t& sentence)
		{
			m_token = new string_view_t(sentence);
			tokenizeLines(m_token);
			tokenizeWords(m_token);
		}
		
		bool Tokenizer::isMultiline(const string_view_t* token, ui32& lineCount)
		{
			const string_view_t::value_type __point { '.' };
			lineCount = contains(token, [ & ](string_view_t::const_reference pt)
			{
				return pt == __point;
			});
			return lineCount > 1;
		}
		
		bool Tokenizer::isComplex(const string_view_t* token, ui32& complexCount)
		{
			const string_view_t::value_type __point { ',' };
			complexCount = contains(token,
			                        [ & ](string_view_t::const_reference pt)
			                        {
				                        return pt == __point;
			                        });
			return complexCount > 0;
		}
		
		ui32
		Tokenizer::countWords(const string_view_t* token)
		{
			ui32 __count { };
			for (ui32 i = 0; i < token->size(); ++i)
			{
				if (token->at(i) == 32)
				{
					++__count;
				}
			}
			return __count;
		}
		
		const std::vector<TokenData*>* Tokenizer::getTokenData() const
		{
			return m_tokenData;
		}
		
		bool Tokenizer::tokenizeLines(const string_view_t* token)
		{
			const ui32 delim = 46;
			ui32 lines { };
			if (!isMultiline(token, lines))
			{
				return false;
			}
			
			m_mode = TokenMode::kMultiLine;
			std::vector<string_view_t*> __tokens { };
			readToken(token, delim, lines, __tokens);
			m_tokenData->reserve(lines);
			if (!__tokens.empty())
			{
				for (const auto& __token : __tokens)
				{
					m_tokenData->push_back(new TokenData(__token));
					auto __tokInfo = m_tokenData->back()->getInfo();
					__tokInfo.lines = lines;
					m_tokenData->back()->setInfo(__tokInfo);
				}
			}
			return !m_tokenData->back()->isEmpty();
		}
		
		bool Tokenizer::tokenizeComplex(const string_view_t* token)
		{
			return false;
			ui32 parts { };
			if (!isComplex(token, parts))
			{
				return false;
			}
			std::vector<string_view_t> __tokens { };
			TokenInfo __info { };
			if (m_tokenData->empty())
			{
				m_tokenData->push_back(new TokenData());
			}
			
			__info = m_tokenData->back()->getInfo();
			__info.punctuationChars = parts;
			
			m_tokenData->back()->setInfo(__info);
			tokenizeWords(token);
			return true;
		}
		
		bool Tokenizer::tokenizeWords(const string_view_t* token)
		{
			const i8 delim = 32;
			const i8 sep = ',';
			std::vector<string_view_t*> __tokens { };
			TokenInfo __info;
			if (m_mode == TokenMode::kMultiLine)
			{
				m_mode = TokenMode::kSimple;
				__info = m_tokenData->back()->getInfo();
				__info.wordCount = countWords(token);
				__info.punctuationChars = std::count(token->begin(), token->end(), sep);
				if (__info.wordCount == 0)
				{
					return false;
				}
				m_tokenData->back()->setInfo(__info);
				readToken(token, delim, __info.wordCount + 1, __tokens);
				for (auto& __tok : __tokens)
				{
					m_tokenData->back()->push(trimExcluded(*__tok), __info);
				}
			} else
			{
				static ui32 line { };
				m_mode = TokenMode::kSimple;
				if (m_tokenData->empty())
				{
					m_tokenData->push_back(new TokenData(const_cast<string_view_t*>(token)));
					m_tokenData->reserve(1);
				}
				for (auto& data : *m_tokenData)
				{
					const ui32 __wordCount = countWords(data->getPart());
					readToken(data->getPart(), delim, __wordCount + 1, __tokens);
					__info = TokenInfo();
					__info.wordCount = __wordCount;
					__info.punctuationChars = std::count(token->begin(), token->end(), sep);
					__info.lines = line++;
					
					for (auto& __tok : __tokens)
					{
						data->push(trimExcluded(*__tok), __info);
					}
				}
			}
			return true;
		}
		
		void Tokenizer::readToken(const string_view_t* token, char delim,
		                          const i64& maxCount, std::vector<string_view_t*>& tokens)
		{
			if (token->empty() || maxCount == 0)
			{
				return;
			}
			
			size_t __pos { };
			size_t __end { };
			
			while ((__end = token->find_first_of(delim, __pos)) <= token->length())
			{
				tokens.push_back(new string_view_t(trimExcluded(range(*token, __pos, __end - __pos))));
				__pos = std::min(__end + 1, token->length());
			}
			
			if (m_mode == TokenMode::kSimple)
			{
				tokens.push_back(new string_view_t(trimExcluded(
						range(*token, __pos, token->length() - __pos - 1)
				)));
			}
		}
		
		string_view_t& Tokenizer::trimExcluded(string_view_t& token)
		{
			return trim(token, nonAllowedChars);
		}
		
		string_view_t& Tokenizer::trimExcluded(string_view_t&& token)
		{
			return trim(token, nonAllowedChars);
		}
		
		string_view_t& Tokenizer::trim(string_view_t& token, const int* charList)
		{
			int index = 0;
			int chr = 0;
			while ((chr = charList[index]) != -1)
			{
				token.remove_prefix(std::min(token.find_first_not_of(chr), token.size()));
				token.remove_suffix(token.size() - std::min(token.find(chr), token.size()));
				++index;
			}
			return token;
		}
		
		inline ui32 Tokenizer::contains(const string_view_t* token, std::function<bool(string_view_t::reference)>&& predicate)
		{
			return static_cast<ui32>(std::count_if(token->cbegin(), token->cend(), predicate));
		}
		
		bool Tokenizer::empty(string_view_t::const_pointer token)
		{
			int index = 0;
			i32 ch = 0;
			while ((ch = token[index]) != '\0')
			{
				if (!std::isalnum(ch))
				{
					return false;
				}
				++index;
			}
			return true;
		}
	}
}
