/**
 *  File SearchEntry.cpp
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QDebug>
#include "Configuration.hpp"

#include "Gui/SearchEntry.hpp"


namespace Dixter
{
	namespace Gui
	{
		SearchEntry::SearchEntry(QWidget* parent, const QString& placeholder,
		                         const QSize& size, int margin)
				: QLineEdit(parent),
				  m_isPlaceholderSet { },
				  m_placeholder { placeholder }
		{
			setTextMargins(margin, margin, margin, margin);
			if (size.width() > 0 && size.height() > 0)
			{
				setMinimumSize(size);
			}
			setPlaceholderText(placeholder);
			connectEvents();
		}
		
		SearchEntry::~SearchEntry()
		{ }
		
		bool SearchEntry::isPlaceholderSet() const
		{
			return m_isPlaceholderSet;
		}
		
		void SearchEntry::onEnter(const QString& text)
		{
			qDebug() << text;
		}
		
		void SearchEntry::connectEvents()
		{
			// BIND_EVENT(wxEVT_TEXT, &SearchEntry::onEnter)
			connect(this, SIGNAL(textChanged(const QString&)),
					SLOT(onEnter(const QString&)));
		}
	}
}