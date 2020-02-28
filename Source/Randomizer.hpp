/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <random>
#include "Commons.hpp"

namespace Dixter
{
	namespace Utilities
	{
		template<
				typename TDistribution = std::uniform_real_distribution<double>,
				typename TEngine = std::default_random_engine,
				typename TResult = typename TDistribution::result_type
		>
		class TRandomGenerator final : public TNonCopyable
		{
		public:
			explicit TRandomGenerator(TResult lowerBound, TResult upperBound)
					: m_randomEngine(new TEngine(m_randomDevice())),
					  m_distribution(new TDistribution(lowerBound, upperBound))
			{ }
			
			explicit TRandomGenerator(TResult upperBound = TResult(1))
					: TRandomGenerator(TResult(0), upperBound)
			{ }
			
			~TRandomGenerator() noexcept
			{
				delete m_randomEngine;
				delete m_distribution;
			}
			
			TResult getRand()
			{
				return ( *m_distribution )(*m_randomEngine);
			}
			
			inline void setSeed(UInt32 seed = TEngine::default_seed) noexcept
			{
				m_randomEngine->seed(seed);
			}
			
			template<class Container>
			inline void generate(Container& container, TSize n = 0) noexcept
			{
				TSize idx = 0;
				while (idx++ != n)
					container.push_back(getRand());
			}
		
		private:
			std::random_device m_randomDevice;
			
			TEngine* m_randomEngine;
			
			TDistribution* m_distribution;
		};
	} // namespace Utilities
} // namespace Dixter