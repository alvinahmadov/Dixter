/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Commons.hpp"


namespace Dixter
{
	template<typename T>
	const size_t ClassInfo<T>::ClassNameConvertor::m_charTableSize = 10;
	
	template<typename T>
	std::list<const byte*>
			ClassInfo<T>::ClassNameConvertor::m_charTable
			{
					"N3", "NSt7"
			};
	
	template<typename T>
	std::map<string_t, string_t>
			ClassInfo<T>::ClassNameConvertor::m_names
			{
					{"NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", "string"},
					{"St17basic_string_viewIcSt11char_traitsIcEE",           "string_view"},
					{"8wxString",                                            "wxString"},
					{"d",                                                    "double"},
					{"f",                                                    "float"},
					{"l",                                                    "long"},
					{"i",                                                    "int"},
					{"c",                                                    "char"},
			};
	
	template<typename T>
	string_t
	ClassInfo<T>::ClassNameConvertor::getName(const string_t& rawName)
	{
		return _getName(rawName.c_str());
	}
	
	template<typename T>
	string_t
	ClassInfo<T>::ClassNameConvertor::getName(const char* rawName)
	{
		return _getName(rawName);
	}
	
	template<typename T>
	string_t
	ClassInfo<T>::ClassNameConvertor::parseName(const char* raw)
	{
		size_t __size {0};
		string_t res {};
		bool __start {true};
		while (raw[__size] != '\0')
		{
			if (raw[__size + 1] == '\0' && raw[__size] == 'E')
				break;
			if (isalpha(raw[__size]))
			{
				res.push_back(raw[__size]);
			} else if (isdigit(raw[__size++]))
			{
				if (!__start)
					res.append("::");
				while (isdigit(raw[__size]) && raw[__size] != '\0')
				{}
				__start = false;
			}
			++__size;
		}
		
		return string_t();
	}
	
	template<typename T>
	string_t
	ClassInfo<T>::ClassNameConvertor::_getName(const char* rawName)
	{
		auto __iter = m_names.find(rawName);
		if (__iter != m_names.end())
			return m_names.find(rawName)->second;
		return parseName(rawName);
	}
	
	Exception::Exception(const string_t& message) dxDECL_NOEXCEPT
			: std::exception(),
			  m_message {message}
	{}
	
	Exception::~Exception() dxDECL_NOEXCEPT
	{}
	
	inline const char* Exception::what() const dxDECL_NOEXCEPT
	{
		return m_message.c_str();
	}
	
	inline const string_t&
	Exception::getMessage() const dxDECL_NOEXCEPT
	{
		return m_message;
	}

	#define DEF_DETAILED_EXCEPTION(className)                   \
    className::className(const string_t& message)               \
        : Exception(message) {}                                 \
    className::~className() dxDECL_NOEXCEPT {}                  \
    const char* className::what() const dxDECL_NOEXCEPT         \
    {                                                           \
        return m_message.data();                                \
    }                                                           \
    const string_t& className::getMessage()                     \
    const dxDECL_NOEXCEPT                                       \
    {                                                           \
        return Exception::getMessage();                         \
    }                                                           \

	DEF_DETAILED_EXCEPTION(IllegalArgumentException)
	
	DEF_DETAILED_EXCEPTION(NotImplementedException)
	
	DEF_DETAILED_EXCEPTION(NullPointerException)
	
	DEF_DETAILED_EXCEPTION(NotFoundException)
	
	DEF_DETAILED_EXCEPTION(RangeException)
	
	DEF_DETAILED_EXCEPTION(SQLException)
	
	size_t DJBHash::operator()(const string_t& hashKey) const
	{
		size_t hash = 5381;
		for (size_t i = 0; i < hashKey.size(); ++i)
		{
			hash = ((hash << 5) + hash) + hashKey.at(i);
		}
		return hash;
	}
}