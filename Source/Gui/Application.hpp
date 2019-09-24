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

#include "Window.hpp"

namespace Dixter
{
	namespace Gui
	{
		class TApplication : public QApplication
		{
		Q_OBJECT
		public:
			explicit TApplication(int& argc, char** argv,
								  const QString& appId);
			
			~TApplication() override;
			
			int exec(QWidget* window);
		};
	}
}