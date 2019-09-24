/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "Exception.hpp"

namespace Dixter
{
	TException::TException(const char* message) noexcept
			: std::exception(),
			  m_message(message)
	{ }
	
	TException::TException(TString message) noexcept
			: std::exception(),
			  m_message(message)
	{ }
	
	const char*
	TException::what() const noexcept
	{
		return m_message.c_str();
	}
	
	const TString&
	TException::getMessage() noexcept
	{
		return m_message;
	}
	
	#define DEF_DETAILED_EXCEPTION(className)                   \
	className::className(TString message) noexcept				\
	: TException(message) {}									\
    const char* className::what() const noexcept                \
    {                                                           \
        return m_message.data();                                \
    }                                                           \
    const TString&                                              \
    className::getMessage() noexcept                            \
    {                                                           \
        static bool caughtOnce(false);                          \
        if (not caughtOnce) {                                   \
         m_message.insert(0, "Caught: ");                       \
         caughtOnce = true;                                     \
        };                                                      \
        return m_message;                                       \
    }                                                           \

	DEF_DETAILED_EXCEPTION(TIllegalArgumentException)
	
	DEF_DETAILED_EXCEPTION(TNotImplementedException)
	
	DEF_DETAILED_EXCEPTION(TNullPointerException)
	
	DEF_DETAILED_EXCEPTION(TNotFoundException)
	
	DEF_DETAILED_EXCEPTION(TRangeException)
	
	DEF_DETAILED_EXCEPTION(TSQLException)
}