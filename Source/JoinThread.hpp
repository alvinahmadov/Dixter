/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <thread>
#include <mutex>

namespace Dixter
{
	/**
	 * C++20 std::jthread implementation
	 * */
	class JoinThread
	{
		using TThread = std::thread;
	public:
		JoinThread() = delete;
		
		template<
				typename TCallable,
				typename... TArgs
		>
		explicit JoinThread(TCallable&& callable, TArgs&& ... args) noexcept;
		
		explicit JoinThread(TThread thread) noexcept;
		
		JoinThread(JoinThread&& other) noexcept;
		
		~JoinThread() noexcept;
		
		JoinThread& operator=(JoinThread&& other) noexcept;
		
		JoinThread& operator=(JoinThread other) noexcept;
		
		void swap(JoinThread& other) noexcept;
		
		TThread::id getId() const noexcept;
		
		bool joinable() const noexcept;
		
		void join();
		
		void detach();
		
		TThread& thread() noexcept;
		
		const TThread& thread() const noexcept;
	
	private:
		TThread m_thread;
	};
	
	template<
	        typename TCallable,
	        typename... TArgs
	        >
	JoinThread::JoinThread(TCallable&& callable, TArgs&& ... args) noexcept
			: m_thread(std::forward<TCallable>(callable), std::forward<TArgs>(args)...)
	{ }
} // namespace Dixter