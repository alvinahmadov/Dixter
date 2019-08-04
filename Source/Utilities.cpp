/**
 *  File
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <cmath>
#include <codecvt>
#include "Utilities.hpp"
#include "Macros.hpp"

namespace Dixter
{
	namespace Utilities
	{
		wstring_t Strings::toWstring(const std::string& str)
		{
			wstring_t __val { };
			for (const auto& chr: str)
				__val.append(1, chr);
			
			return __val;
		}
		
		int Strings::wstrToInt(const std::wstring& value)
		{
			int number = 0;
			for (size_t i = 0; i < value.size(); ++i)
				number += std::pow(10, value.size() - i) * (int) value.at(i);
			
			return number;
		}
		
		int Strings::uStringToInt(const ustring_t& value)
		{
			(void)value;
			// return static_cast<int>(std::strtol(value.c_cstr(), NULL, 0));
			return 1;
		}
		
		ustring_t Strings::intToUstring(int value)
		{
			ustring_t __val {};
			__val << value;
			return __val;
		}
	}
}