/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "SettingsController.hpp"
#include "Configuration.hpp"
#include "Exception.hpp"
#include "Constants.hpp"
#include "Types.hpp"

namespace Dixter
{
	static std::set<TString> g_confPath { g_langConfigPath, g_voiceConfigPath, g_guiConfigPath };
	
	SettingsController::SettingsController(const TString& configRoot) noexcept
			: m_root(configRoot),
			  m_type(EConfiguration::None)
	{
		try
		{
			m_type = getManager(EConfiguration::XML, g_confPath)->getType();
		}
		catch (TException& e)
		{
			printerr(e.getMessage())
		}
	}
	
	void SettingsController::read(const TString& key, TUString& value)
	{
		value = getManager(m_type, g_confPath)
				->accessor()
				->getValue(key, m_root);
	}
	
	void SettingsController::write(const TString& key, const TUString& value)
	{
		getManager(m_type, g_confPath)
				->mutator()
				->setValue(key, value, m_root);
	}
}
