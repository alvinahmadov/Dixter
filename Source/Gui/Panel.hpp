/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QWidget>

namespace Dixter
{
	namespace Gui
	{
		class TOptionBox;
		
		class APanel : public QWidget
		{
		Q_OBJECT
		protected:
			using TOptionBoxPtr = std::shared_ptr<TOptionBox>;
		protected:
			APanel(QWidget* parent, const QSize& size = QSize(200, 200));
			
			virtual ~APanel() = default;
		
			virtual void init() = 0;
			
			virtual void connectEvents() = 0;
		};
	}
}