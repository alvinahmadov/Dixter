/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "Button.hpp"
#include <Commons.hpp>
#include <Configuration.hpp>

namespace Dixter
{
	namespace Gui
	{
		//Button implementation
		Button::Button(QWidget* parent, const QString& label)
				: QPushButton(label, parent)
		{
		}
		
		Button::Button(QIcon icon, QWidget *parent)
				: Button(parent,QString())
		{
			icon.actualSize(size());
			setIcon(icon);
		}
		
		Button::Button(const QString& label)
				: Button(nullptr, label)
		{
		}
		
		Button::~Button()
		{
		}
	}
}