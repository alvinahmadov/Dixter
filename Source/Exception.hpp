/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <exception>
#include "Commons.hpp"

namespace Dixter
{
	//TODO: Extend exception class to have a stackTrace functionality and detailed information.
	class TException : public std::exception
	{
	public:
		TException() noexcept = default;
		
		TException(const char* message) noexcept;
		explicit TException(TString message) noexcept;
		
		virtual ~TException() noexcept override = default;
		
		virtual const char* what() const noexcept override;
		
		virtual const TString& getMessage() noexcept;
	
	protected:
		TString m_message;
	};
	
	#define DECL_DETAILED_EXCEPTION(className)                          \
    class className : public TException                                 \
    {                                                                   \
    public:                                                             \
		explicit className(TString message) noexcept;					\
        template <typename ... Args>                                    \
        className(const char* message, Args&& ... args)                 \
            : TException()                                              \
            {                                                           \
               m_message = TString(message);   		                	\
               VarArgMessageFormat<Args...>::                           \
                       format(m_message,                                \
                       		std::forward<Args>(args)...);               \
            };                                                          \
        ~className() noexcept override = default;                       \
        const char* what() const noexcept override;                     \
        const TString& getMessage() noexcept override;                 	\
    };
	
	DECL_DETAILED_EXCEPTION(TNotImplementedException)
	
	DECL_DETAILED_EXCEPTION(TIllegalArgumentException)
	
	DECL_DETAILED_EXCEPTION(TNullPointerException)
	
	DECL_DETAILED_EXCEPTION(TNotFoundException)
	
	DECL_DETAILED_EXCEPTION(TRangeException)
	
	DECL_DETAILED_EXCEPTION(TSQLException)
} // namespace Dixter