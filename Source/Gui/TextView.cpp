/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "TextView.hpp"

namespace Dixter
{
	namespace Gui
	{
		TTextView::TTextView(QWidget* parent, int rows, int columns)
				: QTableWidget(rows, columns, parent)
		{
			connectEvents();
		}
		
		TTextView::TTextView(QWidget* parent, bool enabled)
				: TTextView(parent, 0, 0)
		{
			setEnabled(enabled);
		}
		
		void TTextView::insertColumns(const QStringList& columns, const QString&)
		{
			setHorizontalHeaderLabels(columns);
		}
		
		void TTextView::setColumnText(int columnIndex, const TString& text)
		{
			auto __item = new TViewItem(QString::fromStdString(text));
			__item->setWhatsThis(QString::fromStdString(text));
			setHorizontalHeaderItem(columnIndex, __item);
		}
		
		void TTextView::setRowText(int rowIndex, int columnIndex, const TString& text)
		{
			if (isSortingEnabled())
				setSortingEnabled(false);
			auto __item = new TViewItem(QString::fromStdString(text));
			__item->setTextAlignment(1);
			setItem(rowIndex, columnIndex, __item);
			setColumnWidth(columnIndex, text.length() * 20);
		}
		
		void TTextView::clearAll()
		{
			clear();
			
			setRowCount(0);
			setColumnCount(0);
		}
		
		void TTextView::connectEvents()
		{ }
	}
}