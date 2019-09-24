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
		TButton::TButton(QWidget* parent, const QString& label)
				: QPushButton(label, parent)
		{
		}
		
		TButton::TButton(QIcon icon, QWidget* parent)
				: TButton(parent, QString())
		{
			icon.actualSize(size());
			setIcon(icon);
		}
		
		TButton::TButton(const QString& label)
				: TButton(nullptr, label)
		{
		}
		
		TButton::~TButton()
		{ }
	}
}