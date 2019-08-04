/**
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *  Copyright (C) 2015-2019
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "TextEntry.hpp"

namespace Dixter
{
	namespace Gui
	{
		TextEntry::TextEntry(QWidget* parent, const QString& value,
		                     const QSize& size, const QString& name)
				: QLineEdit(value, parent)
		{
			setObjectName(name);
			setFixedSize(size);
			connectEvents();
			setVisible(true);
		}
		
		/*
		void TextEntry::onEntryChanged(wxKeyEvent& event)
		{
			printl_log(event.m_keyCode)
			event.Skip();
		}
		*/
		
		void TextEntry::connectEvents()
		{
			// Bind(wxEVT_CHAR, &TextEntry::onEntryChanged, this);
		}
	}
}