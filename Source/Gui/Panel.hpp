/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Constants.hpp"

#include <QWidget>

namespace Dixter
{
	namespace Gui
	{
		class Panel : public QWidget
		{
		Q_OBJECT
		public:
			explicit Panel(QWidget* parent, const QSize& size = QSize(200, 200),
			               const QString& name = "Panel");
			
			~Panel() dxDECL_OVERRIDE;
			
			// virtual QWidget* getWidget(WidgetID id) = 0;
		
		protected:
			virtual void connectEvents();
		};
	}
}