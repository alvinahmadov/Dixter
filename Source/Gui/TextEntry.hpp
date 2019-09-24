/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QLineEdit>

#include "Types.hpp"

namespace Dixter
{
	namespace Gui
	{
		class TTextEntry : public QLineEdit
		{
		Q_OBJECT
		public:
			TTextEntry(QWidget* parent,
			           const QString& value = QString(),
			           const QSize& size = QSize(100, 100),
			           const QString& name = QString());
			
			// virtual void onEntryChanged(wxKeyEvent& event);
			
			void connectEvents();
		};
	}
}