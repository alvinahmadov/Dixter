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
		TApplication::TApplication(int& argc, char** argv,
								   const QString& appId)
				: QApplication(argc, argv)
		{
			setApplicationName(appId);
		}
		
		TApplication::~TApplication()
		{ }
		
		int TApplication::exec(QWidget* window)
		{
			try
			{
				setActiveWindow(window);
				window->show();
				return QApplication::exec();
			} catch (std::exception& e)
			{
				printerr(e.what())
			}
		}
	}
}