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
	class JoinThread
	{
		using Thread = std::thread;
	public:
		template<typename Callable, typename ... Args>
		explicit JoinThread(Callable&& callable, Args&& ... args) noexcept;
		
		explicit JoinThread(Thread thread) noexcept;
		
		JoinThread(JoinThread&& other) noexcept;
		
		~JoinThread() noexcept;
		
		JoinThread& operator=(JoinThread&& other) noexcept;
		
		JoinThread& operator=(JoinThread other) noexcept;
		
		void swap(JoinThread& other) noexcept;
		
		Thread::id getId() const noexcept;
		
		bool joinable() const noexcept;
		
		void join();
		
		void detach();
		
		Thread& thread() noexcept;
		
		const Thread& thread() const noexcept;
	
	private:
		Thread m_thread;
	};
	
	template<typename Callable, typename ... Args>
	JoinThread::JoinThread(Callable&& callable, Args&& ... args) noexcept
			: m_thread(std::forward<Callable>(callable), std::forward<Args>(args)...)
	{ }
}