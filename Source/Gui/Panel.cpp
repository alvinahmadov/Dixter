/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Commons.hpp"
#include "Configuration.hpp"
#include "Panel.hpp"

namespace Dixter
{
	namespace Gui
	{
		Panel::Panel(QWidget* parent, const QSize& size, const QString& name)
				: QWidget(parent)
		{
			connectEvents();
			setFixedSize(size);
			setObjectName(name);
		}
		
		Panel::~Panel()
		{ }
		
		void Panel::connectEvents()
		{
		}
	}
}