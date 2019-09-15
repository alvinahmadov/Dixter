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
		
		#ifdef HAVE_CXX17
		
		std::ostream&
		operator<<(std::ostream& out, const std::vector<TToken*>& pTokens)
		{
			for (const auto& tokenData : pTokens)
			{
				if (tokenData->getChunks())
				{
					// printl_log(tokenData->getInfo())
					for (const auto& token : tokenData->getChunks().value())
					{
						out << token;
						out << std::endl;
					}
				}
			}
			return out;
		}
		
		std::ostream&
		operator<<(std::ostream& out, const TTokenizer* tokenizer)
		{
			out << tokenizer->getTokens();
			return out;
		}
		
		inline void removePrefix(TStringView& strView, size_t n)
		{
			strView.remove_prefix(n);
		};
		
		inline void removeSuffix(TStringView& strView, size_t n)
		{
			strView.remove_suffix(n);
		};
		
		inline TStringView range(const TStringView& stringView, size_t begin, size_t end)
		{
			auto len = std::min(end, stringView.length());
			return TStringView(stringView.data() + begin, len);
		}
		
		#else
		
		inline void removePrefix(TString& str, size_t n)
		{
			dxUNUSED(str)
			dxUNUSED(n)
		};
		
		inline void removeSuffix(TString& str, size_t n)
		{
			dxUNUSED(str)
			dxUNUSED(n)
		};
		
		inline TString
		range(const TString& string, size_t begin, size_t end)
		{
			auto len = std::min(end, string.length());
			return TString{string.data() + begin, len};
		}
		
		#endif
		
		///TokenData Impl.
		std::ostream& operator<<(std::ostream& out, const TToken::TokenInfo& info)
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
		
		
		TString tokenModeToString(TokenMode mode)
		{
			TString ret { };
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
		
		TToken::TToken(TConstValue part) noexcept
				: m_token { part },
				  m_chunks { new TTokenValueHolder{} }
		{ }
		
		TToken::TToken(const TToken& o) noexcept
				: CopyConstructible(o),
				  m_token { o.m_token },
				  m_chunks { new TTokenValueHolder(*o.m_chunks) }
		{ }
		
		TToken::~TToken() noexcept
		{
			delete m_chunks;
		}
		
		bool TToken::push(TConstValue chunk, const TokenInfo& info)
		{
			if (!chunk.empty())
			{
				m_chunks->tokens.push_back(chunk);
				m_chunks->tokenInfo = info;
				return true;
			}
			
			return false;
		}
		
		inline bool TToken::isEmpty() const
		{
			return m_chunks->tokens.empty();
		}
		
		inline size_t TToken::size() const
		{
			return m_chunks->tokens.size();
		}
		
		TOptional<TToken::TTokenVector>
		TToken::getChunks() const
		{
			TOptional<TToken::TTokenVector> ret {};
			ret = m_chunks->tokens;
			return ret;
		}
		
		TToken::TConstValue
		TToken::getChunk(size_t index) const
		{
			if (m_chunks->tokens.size() == 1)
				return m_chunks->tokens.back();
			
			if (m_chunks->tokens.size() > 1)
				return m_chunks->tokens.at(index);
			
			if (index > m_chunks->tokens.size())
				throw RangeException("Index %d > size: %d.", index, m_chunks->tokens.size());
			
			throw NotFoundException("%s:%d View by index %d not found", __FILE__, __LINE__, index);
		}
		
		TToken::TConstValue
		TToken::getPart() const
		{
			return m_token;
		}
		
		const TToken::TokenInfo&
		TToken::getInfo() const
		{
			return m_chunks->tokenInfo;
		}
		
		TToken::TokenInfo&
		TToken::getInfo()
		{
			return m_chunks->tokenInfo;
		}
		
		void TToken::setInfo(const TokenInfo& info)
		{
			m_chunks->tokenInfo = info;
		}
		
		///Tokenizer Impl.
		TTokenizer::TTokenizer()
				: m_mode { TokenMode::kNone },
				  m_token {},
				  m_tokens {}
		{
		}
		
		TTokenizer::~TTokenizer()
		{
			for (auto& t : m_tokens)
				delete t;
		}
		
		void TTokenizer::tokenize(TConstValue sentence)
		{
			m_token = sentence;
			tokenizeLines(m_token);
			tokenizeWords(m_token);
		}
		
		bool TTokenizer::isMultiline(TConstValue token, UInt32& lineCount) const
		{
			const TValue::value_type __point { '.' };
			lineCount = contains(token, [ & ](TValue::const_reference pt)
			{
				return pt == __point;
			});
			return lineCount > 1;
		}
		
		bool TTokenizer::isComplex(TConstValue token, UInt32& complexCount) const
		{
			const TValue::value_type __point { ',' };
			complexCount = contains(token,
			                        [ & ](TValue::const_reference pt)
			                        {
				                        return pt == __point;
			                        });
			return complexCount > 0;
		}
		
		UInt32
		TTokenizer::countWords(TConstValue token)
		{
			UInt32 __count { };
			for (UInt32 i = 0; i < token.size(); ++i)
			{
				if (token.at(i) == 32)
					++__count;
			}
			return __count;
		}
		
		const std::vector<TToken*>&
		TTokenizer::getTokens() const
		{
			return m_tokens;
		}
		
		TString
		TTokenizer::toString() const
		{
			std::ostringstream str{};
	
			if (not getTokens().empty())
			for (const auto& tokenData : getTokens())
			{
				auto __chunks {tokenData->getChunks()};
				
				#ifdef HAVE_CXX17
				if (__chunks)
				#else
				if (not __chunks.empty())
				#endif
				{
					size_t index{};
					#ifdef HAVE_CXX17
					for (const auto& __chunk : *__chunks)
					#else
					for (const auto& __chunk : __chunks)
					#endif
					{
						str << __chunk;
						str << std::endl;
						++index;
					}
				}
			}
			return str.str();
		};
		
		bool TTokenizer::tokenizeLines(TConstValue token)
		{
			const UInt32 delim = 46;
			UInt32 lines { };
			if (not isMultiline(token, lines))
				return false;
			
			m_mode = TokenMode::kMultiLine;
			std::vector<TValue*> __tokens { };
			readToken(token, delim, lines, __tokens);
			m_tokens.reserve(lines);
			if (not __tokens.empty())
			{
				for (const auto& __token : __tokens)
				{
					m_tokens.push_back(new TToken(*__token));
					auto __tokInfo = m_tokens.back()->getInfo();
					__tokInfo.lines = lines;
					m_tokens.back()->setInfo(__tokInfo);
				}
			}
			return not m_tokens.back()->isEmpty();
		}
		
		bool TTokenizer::tokenizeComplex(TConstValue token)
		{
			UInt32 parts { };
			if (not isComplex(token, parts))
				return false;
			
			std::vector<TValue> __tokens { };
			TToken::TokenInfo __info { };
			// if (m_tokens.empty())
			// 	m_tokens.push_back(new TToken());
			
			__info = m_tokens.back()->getInfo();
			__info.punctuationChars = parts;
			
			m_tokens.back()->setInfo(__info);
			tokenizeWords(token);
			return true;
		}
		
		bool TTokenizer::tokenizeWords(TConstValue token)
		{
			const Int8 delim = 32;
			const Int8 sep = ',';
			std::vector<TValue*> __tokens { };
			TToken::TokenInfo __info;
			if (m_mode == TokenMode::kMultiLine)
			{
				m_mode = TokenMode::kSimple;
				__info = m_tokens.back()->getInfo();
				__info.wordCount = countWords(token);
				__info.punctuationChars = std::count(token.cbegin(), token.cend(), sep);
				
				if (__info.wordCount == 0)
					return false;
				
				m_tokens.back()->setInfo(__info);
				readToken(token, delim, __info.wordCount + 1, __tokens);
				for (auto& __tok : __tokens)
				{
					sanitizeExcluded(*__tok);
					m_tokens.back()->push(*__tok, __info);
				}
			} else
			{
				static UInt32 line { };
				m_mode = TokenMode::kSimple;
				if (m_tokens.empty())
				{
					m_tokens.push_back(new TToken(token));
					m_tokens.reserve(1);
				}
				for (auto& data : m_tokens)
				{
					auto part = data->getPart();
					const UInt32 __wordCount = countWords(part);
					readToken(part, delim, __wordCount + 1, __tokens);
					__info = TToken::TokenInfo();
					__info.wordCount = __wordCount;
					__info.punctuationChars = std::count(token.cbegin(), token.cend(), sep);
					__info.lines = line++;
					
					for (auto& __tok : __tokens)
					{
						sanitizeExcluded(*__tok);
						data->push(*__tok, __info);
					}
				}
			}
			return true;
		}
		
		void TTokenizer::readToken(TConstValue token, char delim,
		                           const Int64& maxCount, std::vector<TValue*>& tokens)
		{
			if (token.empty() or maxCount == 0)
				return;
			
			size_t __pos { };
			size_t __end { };
			
			while ((__end = token.find_first_of(delim, __pos)) <= token.length())
			{
				auto __range = range(token, __pos, __end - __pos);
				sanitizeExcluded(__range);
				tokens.push_back(new TValue(__range));
				__pos = std::min(__end + 1, token.length());
			}
			
			if (m_mode == TokenMode::kSimple)
			{
				auto __range = range(token, __pos, token.length() - __pos - 1);
				sanitizeExcluded(__range);
				tokens.push_back(new TValue(__range));
			}
		}
		
		void
		TTokenizer::sanitizeExcluded(TValue& token)
		{
			sanitize(token, nonAllowedChars);
		}
		
		void
		TTokenizer::sanitizeExcluded(TValue&& token)
		{
			sanitize(token, nonAllowedChars);
		}
		
		void
		TTokenizer::sanitize(TValue& token, const int* charList)
		{
			if (not charList)
				return;
			
			int index = 0;
			int chr = 0;
			while ((chr = charList[index++]) != -1)
			{
				removePrefix(token, std::min(token.find_first_not_of(chr), token.size()));
				removeSuffix(token, token.size() - std::min(token.find(chr), token.size()));
			}
		}
		
		inline UInt32
		TTokenizer::contains(TConstValue token,
		                     std::function<bool(TValue::const_reference)>&& predicate)
		{
			return static_cast<UInt32>(std::count_if(token.cbegin(), token.cend(), predicate));
		}
		
		bool TTokenizer::empty(TValue::const_iterator token)
		{
			int index = 0;
			Int32 ch = 0;
			while ((ch = token[index]) != '\0')
			{
				if (not std::isalnum(ch))
					return false;
				
				++index;
			}
			return true;
		}
	}
}
