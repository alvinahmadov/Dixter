/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "FontPicker.hpp"

namespace Dixter
{
	namespace Gui
	{
		FontPicker::FontPicker(QWidget* parent, const QColor& initial, const QSize& size) noexcept
				: QColorDialog(initial, parent)
		{
			init();
			setFixedSize(size);
			connectEvents();
		}
		
		void FontPicker::init()
		{ }
		
		void FontPicker::connectEvents()
		{
			// BIND_EVENT(wxEVT_FONTPICKER_CHANGED, &FontPicker::onFontChange)
		}
		
		/*
		void FontPicker::onFontChange(wxFontPickerEvent& event)
		{
			handleAction<FontPickerAction>(__FUNCTION__, event);
		}
		
		*/
	} // namespace Gui
} // namespace Dixter