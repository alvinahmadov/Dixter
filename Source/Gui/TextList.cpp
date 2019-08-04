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
		TextList::TextList(QWidget* parent, int rows, int columns)
				: QTableWidget(rows, columns, parent),
				  m_rowCount { 0 },
				  m_rows { }
		{
			connectEvents();
		}
		
		TextList::~TextList()
		{ }
		
		void TextList::insertColumns(const QStringList& columns, const QString&)
		{
			// size_t __index = 0;
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
		
		const ui32& TextList::insertRowValues(i64 index, const std::vector<QString>& values)
		{
			insertRow(index);
			int colidx { };
			
			for (const auto& value : values)
			{
				setItem(index, colidx++, new QTableWidgetItem(value));
			}
			return ++m_rowCount;
		}
		
		void TextList::removeRow(const i64& rowIndex)
		{
			
			remove(rowIndex);
			--m_rowCount;
		}
		
		bool TextList::searchRow(const QString&, i64&, const i64&)
		{
			// position = FindItem(start, text, true);
			// return position != -1;
			return false;
		}
		
		void TextList::clearRows()
		{
			// DeleteAllItems();
			// for (auto& __cont : m_rows)
			// {
			// 	SAFE_RELEASE(__cont.item);
			// 	__cont.index = -1;
			// }
			m_rowCount = 0;
		}
		
		i64
		TextList::getSelectedRowIndex()
		{
			// i64 __index { -1 };
			// if (GetSelectedItemCount() == 1)
			// {
			// 	for (i64 idx { }; idx < m_rowCount; idx++)
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
		
		const ui32
		& TextList::getRowCount() const
		{
			return m_rowCount;
		}
		
		QTableWidgetItem*
		TextList::first() const
		{
			return m_rows.front();
		}
		
		QTableWidgetItem*
		TextList::last() const
		{
			return m_rows.back();
		}
		
		void TextList::connectEvents()
		{
			//BIND_EVENT(signal_cursor_changed, TextList::OnCursorChanged)
		}
		
		void TextList::onCursorChanged()
		{
		}
		
		void TextList::insert(i64)
		{
			auto __item = new QTableWidgetItem();
			m_rows.push_back(__item);
		}
		
		void TextList::remove(i64)
		{
			// for (auto __container : m_rows)
			// {
			// 	if (__container.index == index)
			// 	{
			// 		SAFE_RELEASE(__container.item);
			// 		__container.index = -1;
			// 	}
			// }
		}
	}
}