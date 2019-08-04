/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QColorDialog>

namespace Dixter
{
	namespace Gui
	{
		class ColourPicker : public QColorDialog
		{
			Q_OBJECT
		public:
			ColourPicker(QWidget* parent, const QColor& colour = Qt::GlobalColor::lightGray,
						 const QSize& size = QSize());
			
			virtual ~ColourPicker();
		
		protected:
			virtual void connectEvents();
		};
	}
}