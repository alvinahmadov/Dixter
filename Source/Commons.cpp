/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Commons.hpp"
#include "Constants.hpp"

namespace Dixter
{
	template<typename T>
	const TSize ClassInfo<T>::ClassNameConvertor::m_charTableSize = 10;
	
	template<typename T>
	std::list<const TByte*>
			ClassInfo<T>::ClassNameConvertor::m_charTable
			{
					"N3", "NSt7"
			};
	
	template<typename T>
	std::map<TString, TString>
			ClassInfo<T>::ClassNameConvertor::m_names
			{
					{"NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE", "string"},
					{"St17basic_string_viewIcSt11char_traitsIcEE",           "TStringView"},
					{"8wxString",                                            "wxString"},
					{"d",                                                    "double"},
					{"f",                                                    "float"},
					{"l",                                                    "long"},
					{"i",                                                    "int"},
					{"c",                                                    "char"},
			};
	
	template<typename T>
	TString
	ClassInfo<T>::ClassNameConvertor::getName(const TString& rawName)
	{
		return getName(rawName.c_str());
	}
	
	template<typename T>
	TString
	ClassInfo<T>::ClassNameConvertor::getName(const TByte* rawName)
	{
		auto __iter = m_names.find(rawName);
		if (__iter != m_names.end())
			return m_names.find(rawName)->second;
		return parseName(rawName);
	}
	
	template<typename T>
	TString
	ClassInfo<T>::ClassNameConvertor::parseName(const TByte* raw)
	{
		TSize __size {0};
		TString res {};
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
		
		return TString();
	}
}