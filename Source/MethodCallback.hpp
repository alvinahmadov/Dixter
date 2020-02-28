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
	 * \tparam TClass Type of class of callback method.
	 * \tparam TReturn Return type of method.
	 * \tparam TArgs Variable argument types
	 * */
	template<
			typename TClass,
			typename TReturn,
			typename... TArgs
	>
	class TMethodCallback
	{
	public:
		using TPointer   = TClass*;
		using TReference = TClass&;
		
		typedef TReturn (TClass::*FMethod)(TArgs ... args);
	
	public:
		explicit TMethodCallback(FMethod methodPtr);
		
		explicit TMethodCallback(FMethod methodPtr, TPointer objectPtr);
		
		~TMethodCallback();
		
		TReturn operator()(TPointer objectPtr, TArgs&& ... args);
		
		TReturn operator()(TArgs&& ... args);
		
		static TReturn of(TPointer objectPtr, TArgs&& ... args);
		
		static TReturn of(TArgs&& ... args);
	
	private:
		FMethod m_methodPtr;
		
		TPointer m_objectPtr;
	};
} // namespace Dixter

namespace Dixter
{
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TMethodCallback<TClass, TReturn, TArgs...>::
	TMethodCallback(TMethodCallback::FMethod methodPtr)
			: m_methodPtr(methodPtr),
			  m_objectPtr()
	{ }
	
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TMethodCallback<TClass, TReturn, TArgs...>::
	TMethodCallback(TMethodCallback::FMethod methodPtr, TMethodCallback::TPointer objectPtr)
			: m_methodPtr(methodPtr),
			  m_objectPtr(objectPtr)
	{ }
	
	template<
			class TClass,
			class TReturn,
			typename ... Args
	>
	TMethodCallback<TClass, TReturn, Args...>::~TMethodCallback()
	{ }
	
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TReturn TMethodCallback<TClass, TReturn, TArgs...>::
	operator()(TMethodCallback::TPointer objectPtr, TArgs&& ... args)
	{
		return ( objectPtr->*m_methodPtr )(std::forward<TArgs>(args)...);
	}
	
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TReturn TMethodCallback<TClass, TReturn, TArgs...>::operator()(TArgs&& ... args)
	{
		if (m_objectPtr)
			return ( m_objectPtr->*m_methodPtr )(std::forward<TArgs>(args)...);
	}
	
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TReturn TMethodCallback<TClass, TReturn, TArgs...>::
	of(TMethodCallback::TPointer objectPtr, TArgs&& ... args)
	{
		if (objectPtr)
			return TMethodCallback<TClass, TReturn(TArgs...)>()(objectPtr, std::forward<TArgs>(args)...);
	}
	
	template<
			class TClass,
			class TReturn,
			typename... TArgs
	>
	TReturn TMethodCallback<TClass, TReturn, TArgs...>::
	of(TArgs&& ... args)
	{
		return TMethodCallback<TClass, TReturn(TArgs...)>()(std::forward<TArgs>(args)...);
	}
} // namespace Dixter