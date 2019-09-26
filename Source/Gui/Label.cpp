/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Commons.hpp"
#include "Gui/Label.hpp"

namespace Dixter
{
	namespace Gui
	{
		TLabel::TLabel(const QString& label, QWidget* parent, const QSize& size)
				: QLabel(label, parent)
		{
			if (size != QSize())
				setMinimumSize(size);
			
			setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		}
		
		TLabel::TLabel(const QString& label, const QSize& size)
				: TLabel(label, nullptr, size)
		{}
	} // namespace Gui
} // namespace Dixter