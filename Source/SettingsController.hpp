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
	class SettingsController : public NonCopyable
	{
	public:
		explicit SettingsController(const TString& configRoot) noexcept;
		
		~SettingsController() = default;
		
		void read(const TString& key, TUString& value);
		
		void write(const TString& key, const TUString& value);
	
	private:
		const TString& m_root;
		EConfiguration m_type;
	};
}
