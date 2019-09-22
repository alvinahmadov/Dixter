/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "JoinThread.hpp"

namespace Dixter
{
	JoinThread::JoinThread(TThread thread) noexcept
			: m_thread(std::move(thread))
	{ }
	
	JoinThread::JoinThread(JoinThread&& other) noexcept
			: m_thread(std::move(other.m_thread))
	{ }
	
	JoinThread::~JoinThread() noexcept
	{
		if (joinable())
			join();
	}
	
	JoinThread&
	JoinThread::operator=(JoinThread&& other) noexcept
	{
		if (joinable())
			join();
		
		m_thread = std::move(other.m_thread);
		return *this;
	}
	
	JoinThread&
	JoinThread::operator=(JoinThread other) noexcept
	{
		if (joinable())
			join();
		
		m_thread = std::move(other.m_thread);
		return *this;
	}
	
	void JoinThread::swap(JoinThread& other) noexcept
	{
		m_thread.swap(other.m_thread);
	}
	
	JoinThread::TThread::id
	JoinThread::getId() const noexcept
	{
		return m_thread.get_id();
	}
	
	bool JoinThread::joinable() const noexcept
	{
		return m_thread.joinable();
	}
	
	void JoinThread::join()
	{
		m_thread.join();
	}
	
	void JoinThread::detach()
	{
		m_thread.detach();
	}
	
	JoinThread::TThread&
	JoinThread::thread() noexcept
	{
		return m_thread;
	}
	
	const JoinThread::TThread&
	JoinThread::thread() const noexcept
	{
		return m_thread;
	}
}