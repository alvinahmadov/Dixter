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
		
		class TPanel : public QWidget
		{
		Q_OBJECT
		public:
			using TOptionBoxPtr = std::shared_ptr<TOptionBox>;
		public:
			TPanel(QWidget* parent, const QSize& size = QSize(200, 200),
				   const QString& name = "Panel");
			
			virtual ~TPanel() = default;
		
		protected:
			virtual void connectEvents();
		};
	}
}