/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Configuration.hpp"
#include "Gui/Application.hpp"

namespace Dixter
{
	namespace Gui
	{
		Application::Application(int& argc, char** argv,
		                         const QString& appId, bool enableNativeWindow)
				: QApplication(argc, argv),
				  m_enableSelfWindow(enableNativeWindow)
		{
			setApplicationName(appId);
		}
		
		Application::~Application()
		{
			ConfigurationManager::getManager(ConfigurationType::kConfigXml)->release();
		}
		
		int Application::run(QWidget* window)
		{
			try
			{
				setActiveWindow(window);
				return QApplication::exec();
			} catch (std::exception& e)
			{
				printerr(e.what())
			}
		}
	}
}