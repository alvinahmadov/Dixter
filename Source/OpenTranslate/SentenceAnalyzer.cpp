/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "SentenceAnalyzer.hpp"
#include "Utilities.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		namespace StringUtils = Utilities::Strings;
		
		// TTextData implementation
		TSentenceAnalyzer::
		TTextData::TTextData(TSentenceAnalyzer::TSentence sentence)
				: m_tokenizerPtr(new TTokenizer(sentence))
		{ }
		
		const auto& TSentenceAnalyzer::
		TTextData::get() const
		{
			return m_tokenizerPtr->getTokens();
		}
		
		TString
		TSentenceAnalyzer::
		TTextData::toString()
		{
			return m_tokenizerPtr->toString();
		}
		
		// TTextData implementation
		TSentenceAnalyzer::TSentenceAnalyzer()
				: m_textData()
		{ }
		
		void TSentenceAnalyzer::process(TSentenceAnalyzer::TSentence sentence)
		{
			m_textData.clear();
			auto __parts = StringUtils::split<std::vector<TStringView>>(sentence, '.');
			for (const auto& __part : __parts)
				m_textData.push_back(std::unique_ptr<TTextData>(new TTextData(__part)));
		}
		
		TString
		TSentenceAnalyzer::toString()
		{
			std::ostringstream __oss {};
			
			__oss << "[\n";
			for (TSize idx = 0ULL; idx < m_textData.size(); ++idx)
			{
				const auto& __tdata = m_textData.at(idx)->get();
				if (__tdata.size() > 0)
				{
					__oss << "{";
					TSize j = 0ULL;
					for (const auto& __t : __tdata)
					{
						__oss << __t;
						if (++j != __tdata.size())
							__oss << '|';
					}
					__oss << "}\n";
				}
			}
			__oss << "]\n";
			
			return __oss.str();
		}
	} // namespace OpenTranslate
} // namespace Dixter
