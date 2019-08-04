/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

namespace Dixter
{
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
	 * \namespace Dixter
	 * \class MethodCallback
	 * \brief Allows holder structures to call its elements' methods.
	 * \tparam C Type of class of callback method.
	 * \tparam R Return type of method.
	 * \tparam Args Variable argument types
	 * */
	template<
			typename C,
			typename R,
			typename ... Args
	>
	class MethodCallback
	{
	public:
		using pointer_t   = C*;
		using reference_t = C&;
		
		typedef R (C::*method_t)(Args ... args);
	
	public:
		explicit MethodCallback(method_t methodPtr);
		
		explicit MethodCallback(method_t methodPtr, pointer_t objectPtr);
		
		~MethodCallback();
		
		R operator()(pointer_t objectPtr, Args ... args);
		
		R operator()(Args ... args);
		
		static R of(pointer_t objectPtr, Args ... args);
		
		static R of(Args ... args);
	
	private:
		method_t m_methodPtr;
		pointer_t m_objectPtr;
	};
}

namespace Dixter
{
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::MethodCallback(MethodCallback::method_t methodPtr)
			: m_methodPtr {methodPtr},
			  m_objectPtr {nullptr}
	{}
	
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::
	        MethodCallback(MethodCallback::method_t methodPtr, MethodCallback::pointer_t objectPtr)
			: m_methodPtr {methodPtr},
			  m_objectPtr {objectPtr}
	{}
	
	template<
			class T,
			class Return,
			typename ... Args>
	MethodCallback<T, Return, Args...>::~MethodCallback()
	{}
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::operator()(MethodCallback::pointer_t objectPtr, Args... args)
	{
		return (objectPtr->*m_methodPtr)(args...);
	}
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::operator()(Args... args)
	{
		if (m_objectPtr != nullptr)
			return (m_objectPtr->*m_methodPtr)(args...);
	}
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::of(MethodCallback::pointer_t objectPtr, Args... args)
	{
		if (objectPtr != nullptr)
			return MethodCallback<T, Return(Args...)>()(objectPtr, args...);
	}
	
	template<
			class T,
			class Return,
			typename ... Args>
	Return MethodCallback<T, Return, Args...>::of(Args... args)
	{
			return MethodCallback<T, Return(Args...)>()(args...);
	}
}