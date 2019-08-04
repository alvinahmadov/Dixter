/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QApplication>

#include "MainWindow.hpp"

namespace Dixter
{
	namespace Gui
	{
		class Application : public QApplication
		{
		Q_OBJECT
		public:
			explicit Application(int& argc, char** argv,
			                     const QString& appId, bool enableNativeWindow = true);
			
			~Application() dxDECL_OVERRIDE;
			
			int run(QWidget* window);
		
		private:
			bool m_enableSelfWindow;
		};
	}
}