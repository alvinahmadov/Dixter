/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Utilities.hpp"
#include "TextList.hpp"

namespace Dixter
{
	namespace Gui
	{
		TTextList::TTextList(QWidget* parent, int rows, int columns)
				: QTableWidget(rows, columns, parent),
				  m_rowCount { 0 },
				  m_rows {}
		{
			connectEvents();
		}
		
		TTextList::~TTextList()
		{ }
		
		void TTextList::insertColumns(const QStringList& columns, const QString&)
		{
			// TSize __index = 0;
			setHorizontalHeaderLabels(columns);
			// for (const auto& item : columns)
			// {
			// 	if (except.compare(item))
			// 	{
			// 		setHorizontalHeaderItem(__index, new QTableWidgetItem(item));
			// 	}
			// 	++__index;
			// }
		}
		
		const UInt32&
		TTextList::insertRowValues(Int64 index, const std::vector<QString>& values)
		{
			insertRow(index);
			int colidx {};
			
			for (const auto& value : values)
			{
				setItem(index, colidx++, new QTableWidgetItem(value));
			}
			return ++m_rowCount;
		}
		
		void TTextList::removeRow(const Int64& rowIndex)
		{
			
			remove(rowIndex);
			--m_rowCount;
		}
		
		bool TTextList::searchRow(const QString&, Int64&, const Int64&)
		{
			// position = FindItem(start, text, true);
			// return position != -1;
			return false;
		}
		
		void TTextList::clearRows()
		{
			// DeleteAllItems();
			// for (auto& __cont : m_rows)
			// {
			// 	SAFE_RELEASE(__cont.item);
			// 	__cont.m_index = -1;
			// }
			m_rowCount = 0;
		}
		
		Int64
		TTextList::getSelectedRowIndex()
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
		TTextList::getRowCount() const
		{
			return m_rowCount;
		}
		
		QTableWidgetItem*
		TTextList::first() const
		{
			return m_rows.front();
		}
		
		QTableWidgetItem*
		TTextList::last() const
		{
			return m_rows.back();
		}
		
		void TTextList::connectEvents()
		{
			//BIND_EVENT(signal_cursor_changed, TextList::OnCursorChanged)
		}
		
		void TTextList::onCursorChanged()
		{
		}
		
		void TTextList::insert(Int64)
		{
			auto __item = new QTableWidgetItem();
			m_rows.push_back(__item);
		}
		
		void TTextList::remove(Int64)
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