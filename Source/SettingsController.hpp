/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Configuration.hpp"

namespace Dixter
{
	class SettingsController
	{
	public:
		explicit SettingsController(const string_t& configRoot);
		
		~SettingsController();
		
		void read(const string_t& key, ustring_t& value);
		
		void write(const string_t& key, const ustring_t& value);
	
	private:
		const string_t& m_root;
		ConfigurationType m_type;
	};
}
