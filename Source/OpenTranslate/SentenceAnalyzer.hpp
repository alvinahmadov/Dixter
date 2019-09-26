/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Tokenizer.hpp"

namespace Dixter
{
	namespace OpenTranslate
	{
		class TSentenceAnalyzer
		{
		public:
			#ifdef HAVE_CXX17
			using TSentence = TStringView;
			#else
			using TSentence = const TString&;
			#endif
		private:
			class TTextData final : public NonCopyable
			{
			public:
				TTextData(TSentence sentence);
				
				TTextData() noexcept = delete;
				
				~TTextData() noexcept = default;
				
				TTextData(const TTextData&) = delete;
				
				TTextData& operator=(const TTextData&) = delete;
				
				const auto&
				get() const;
				
				TString toString();
			
			private:
				std::shared_ptr<TTokenizer> m_tokenizerPtr;
			};
		
		public:
			using TTextDataPtrVector = std::vector<std::unique_ptr<TTextData>>;
			
			TSentenceAnalyzer();
			
			~TSentenceAnalyzer() = default;
			
			void process(TSentence sentence);
			
			TString toString();
		
		private:
			TTextDataPtrVector m_textData;
		};
	} // namespace OpenTranslate
} // namespace Dixter