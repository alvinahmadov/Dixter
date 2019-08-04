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
		template<typename Distribution = std::uniform_real_distribution<double>,
				typename Engine = std::default_random_engine,
				typename ResultType = typename Distribution::result_type>
		class RandomGenerator : public NonCopyable
		{
		public:
			explicit RandomGenerator(ResultType lowerBound, ResultType upperBound)
					: m_randomDevice {std::random_device()},
					  m_randomEngine(new Engine(m_randomDevice())),
					  m_distribution(new Distribution(lowerBound, upperBound))
			{}
			
			explicit RandomGenerator(ResultType upperBound = ResultType(1))
					: m_randomDevice {std::random_device()},
					  m_randomEngine(new Engine(m_randomDevice())),
					  m_distribution(new Distribution(ResultType(0), upperBound))
			{}
			
			~RandomGenerator()
			{
				SAFE_RELEASE(m_randomEngine)
				SAFE_RELEASE(m_distribution)
			}
			
			ResultType getRand()
			{
				return (*m_distribution)(*m_randomEngine);
			}
			
			void setSeed(ui32 seed = Engine::default_seed)
			{
				m_randomEngine->seed(seed);
			}
			
			template<class Container>
			void generate(Container& container, size_t n = 0)
			{
				size_t idx = 0;
				while (idx++ != n)
					container.push_back(getRand());
			}
		
		private:
			std::random_device m_randomDevice;
			Engine* m_randomEngine;
			Distribution* m_distribution;
		};
	}
}
