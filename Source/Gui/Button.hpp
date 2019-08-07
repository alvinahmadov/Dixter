/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Types.hpp"

#include <QPushButton>


namespace Dixter
{
	namespace Gui
	{
		class Button : public QPushButton
		{
			Q_OBJECT
		public:
			explicit Button(QWidget* parent, const QString& label = QString());
			
			explicit Button(QIcon icon, QWidget *parent = nullptr);
			
			explicit Button(const QString& label);
			
			~Button() dxDECL_OVERRIDE;
		};
	}
}