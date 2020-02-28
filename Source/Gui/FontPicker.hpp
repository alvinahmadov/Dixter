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
		class FontPicker : public QColorDialog
		{
		Q_OBJECT
		public:
			FontPicker(QWidget* parent, const QColor& initial = QColor(),
			           const QSize& size = QSize(300, 300)) noexcept;
		
		protected:
			void init();
			
			virtual void connectEvents();
			
			// void onFontChange(wxFontPickerEvent& event);
		};
	} // namespace Gui
} // namespace Dixter