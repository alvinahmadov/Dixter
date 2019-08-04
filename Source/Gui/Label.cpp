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
#include "Gui/Label.hpp"

namespace Dixter
{
	namespace Gui
	{
		Label::Label(const QString& label, QWidget* parent, const QSize& size)
				: QLabel(label, parent)
		{
			if (size != QSize())
				setMinimumSize(size);
			
			setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		}
		
		Label::Label(const QString& label, const QSize& size)
				: Label(label, nullptr, size)
		{}
		
		Label::~Label()
		{ }
	}
}