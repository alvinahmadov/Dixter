/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Commons.hpp"
#include "Constants.hpp"
#include "Panel.hpp"

namespace Dixter
{
	namespace Gui
	{
		APanel::APanel(QWidget* parent, const QSize& size)
				: QWidget(parent)
		{
			setFixedSize(size);
		}
	}
}