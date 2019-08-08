/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "SettingsController.hpp"

namespace Dixter
{
	SettingsController::SettingsController(const string_t& configRoot)
			: m_root {configRoot}
	{
		m_type = ConfigurationManager::getManager(ConfigurationType::ConfigXml)->getType();
	}
	
	SettingsController::~SettingsController()
	{}
	
	void SettingsController::read(const string_t& key, ustring_t& value)
	{
		value = ConfigurationManager::getManager(m_type)->getAccessor()->getValue(m_root, key);
	}
	
	void SettingsController::write(const string_t& key, const ustring_t& value)
	{
		ConfigurationManager::getManager(m_type)->getMutator()->setValue(m_root, key, value);
	}
}
