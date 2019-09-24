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
		TWString Strings::toWstring(const std::string& str)
		{
			TWString __val { };
			for (const auto& chr: str)
				__val.append(1, chr);
			
			return __val;
		}
		
		int Strings::wstrToInt(const std::wstring& value)
		{
			int number = 0;
			for (TSize i = 0; i < value.size(); ++i)
				number += std::pow(10, value.size() - i) * (int) value.at(i);
			
			return number;
		}
		
		int Strings::uStringToInt(const TUString& value)
		{
			(void)value;
			// return static_cast<int>(std::strtol(m_value.c_cstr(), NULL, 0));
			return 1;
		}
		
		TUString Strings::intToUstring(int value)
		{
			TUString __val {};
			__val << value;
			return __val;
		}
	}
}