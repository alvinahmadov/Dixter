/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "TextView.hpp"
#include "Macros.hpp"
#include "Configuration.hpp"
#include "Types.hpp"
#include "Utilities.hpp"

namespace Dixter
{
	namespace Gui
	{
		TTextView::TTextView(QWidget* parent, int rows, int columns)
				: QTableWidget(rows, columns, parent),
				  m_rowCount { 0 },
				  m_rows {}
		{
			connectEvents();
		}
		
		TTextView::~TTextView()
		{ }
		
		void TTextView::insertColumns(const QStringList& columns, const QString&)
		{
			setHorizontalHeaderLabels(columns);
		}
		
		void TTextView::setColumnText(int columnIndex, const TString& text)
		{
			auto __item = new TViewItem(QString::fromStdString(text));
			setHorizontalHeaderItem(columnIndex, __item);
		}
		
		void TTextView::setRowText(int rowIndex, int columnIndex, const TString& text)
		{
			auto __item = new TViewItem(QString::fromStdString(text));
			setItem(rowIndex, columnIndex, __item);
		}
		
		void TTextView::removeRow(const Int64& rowIndex)
		{
			
			remove(rowIndex);
			--m_rowCount;
		}
		
		bool TTextView::searchRow(const QString&, Int64&, const Int64&)
		{
			// position = FindItem(start, text, true);
			// return position != -1;
			return false;
		}
		
		void TTextView::clearAll()
		{
			clearContents();
			clear();
			
			for (int __index = 0; __index < rowCount(); ++__index)
				removeRow(__index);
			
			setRowCount(0);
			setColumnCount(0);
		}
		
		Int64
		TTextView::getSelectedRowIndex()
		{
			// Int64 __index { -1 };
			// if (GetSelectedItemCount() == 1)
			// {
			// 	for (Int64 idx { }; idx < m_rowCount; idx++)
			// 	{
			// 		auto state = GetItemState(idx, wxLIST_STATE_SELECTED);
			// 		if (state == wxLIST_STATE_SELECTED)
			// 		{
			// 			__index = idx;
			// 			break;
			// 		}
			// 	}
			// }
			// return __index;
		}
		
		const UInt32&
		TTextView::getRowCount() const
		{
			return m_rowCount;
		}
		
		QTableWidgetItem*
		TTextView::first() const
		{
			return m_rows.front();
		}
		
		QTableWidgetItem*
		TTextView::last() const
		{
			return m_rows.back();
		}
		
		void TTextView::connectEvents()
		{ }
		
		void TTextView::onCursorChanged()
		{ }
		
		void TTextView::insert(Int64)
		{
			auto __item = new QTableWidgetItem();
			m_rows.push_back(__item);
		}
		
		void TTextView::remove(Int64)
		{
			// for (auto __container : m_rows)
			// {
			// 	if (__container.m_index == m_index)
			// 	{
			// 		SAFE_RELEASE(__container.item);
			// 		__container.m_index = -1;
			// 	}
			// }
		}
	}
}