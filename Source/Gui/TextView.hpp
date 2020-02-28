/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QTableWidget>

#include "Types.hpp"

namespace Dixter
{
	namespace Gui
	{
		class TTextView : public QTableWidget
		{
		Q_OBJECT
		public:
			using TViewItem = QTableWidgetItem;
		
		public:
			TTextView(QWidget* parent = nullptr, int rows = 0, int columns = 0);
			
			explicit TTextView(QWidget* parent, bool enabled);
			
			~TTextView() = default;
			
			void insertColumns(const QStringList& columns, const QString& except = QString());
			
			void setColumnText(int columnIndex, const TString& text);
			
			void setRowText(int rowIndex, int columnIndex, const TString& text);
			
			void clearAll();
		
		protected:
			virtual void connectEvents();
		};
	}
}