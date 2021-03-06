/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Commons.hpp"

namespace Dixter
{
	enum class EConfiguration;
	
	class TSettingsController : public TNonCopyable
	{
	public:
		explicit TSettingsController(const TString& configRoot) noexcept;
		
		~TSettingsController() = default;
		
		void read(const TString& key, TUString& value);
		
		void write(const TString& key, const TUString& value);
	
	private:
		const TString& m_root;
		EConfiguration m_type;
	};
}
