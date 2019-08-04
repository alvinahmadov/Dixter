/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <memory>
#include "Macros.hpp"
#include "Types.hpp"

namespace Dixter
{
	template<class Object,
			template<class C>
			class PointerType,
			class ... Args>
	class SmartPointer
	{
	public:
		static PointerType<Object>& Create(Args ... args) dxDECL_NOEXCEPT
		{
			m_object = PointerType<Object>(new Object(args...));
			return m_object;
		}
		
		~SmartPointer() dxDECL_NOEXCEPT
		{
			m_object.reset();
		}
	
	private:
		static PointerType<Object> m_object;
	};
	
	template<class Object,
			template<class C>
			class PointerType,
			class ... Args>
	PointerType<Object>
			SmartPointer<Object, PointerType, Args...>::m_object = nullptr;
	
	template<class Object,
			template<class C = Object, class D = std::default_delete<C>>
			class PointerType,
			class ... Args>
	class SmartPointerWithDeleter
	{
	public:
		static PointerType<Object>&& Create(Args ... args)
		{
			return std::move(PointerType<Object>(new Object(args...)));
		}
		
		~SmartPointerWithDeleter() dxDECL_NOEXCEPT
		{
			m_object.reset(nullptr);
		}
	
	protected:
		static PointerType<Object> m_object;
	};
	
	template<class Object,
			template
			<class C, class D>
			class PointerType,
			class ... Args>
	PointerType<Object, std::default_delete<Object>>
			SmartPointerWithDeleter<Object, PointerType, Args...>::m_object = nullptr;
	
	template<class Object,
			class ... Args>
	struct WeakPtr : public SmartPointer<Object, Weak, Args ...>
	{
		static Weak<Object>& create(Args ... args) dxDECL_NOEXCEPT
		{
			return SmartPointer<Object, Weak, Args ...>::Create(args...);
		}
	};
	
	template<class Object,
			class ... Args>
	struct SharedPtr : public SmartPointer<Object, Shared, Args ...>
	{
		static Shared<Object>& create(Args ... args) dxDECL_NOEXCEPT
		{
			return SmartPointer<Object, Shared, Args ...>::Create(args...);
		}
		
		Shared<Object>& operator()(Args ... args)
		{
			return create(args...);
		}
	};
	
	template<class Object,
			class ... Args>
	struct UniquePtr : public SmartPointerWithDeleter<Object, Unique, Args ...>
	{
	};
}
