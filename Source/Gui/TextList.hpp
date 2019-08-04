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
#include <QDebug>

#include "Configuration.hpp"
#include "Types.hpp"
#include "Macros.hpp"

class QTableWidgetItem;
namespace Dixter
{
	namespace Gui
	{
		struct ItemContainer
		{
			QTableWidgetItem* item;
			i64 index;
		};
		class TextList : public QTableWidget
		{
		Q_OBJECT
		public:
			using RowMap = std::map<int, ustring_t>;
		public:
			TextList(QWidget* parent = nullptr, int rows = 0, int columns = 0);
			
			~TextList() dxDECL_OVERRIDE;
			
			void insertColumns(const QStringList& columns, const QString& except = QString());
			
			const ui32&
			insertRowValues(i64 index, const std::vector<QString>& values);
			
			void removeRow(const i64& rowIndex);
			
			bool searchRow(const QString& text, i64& position, const i64& start = -1);
			
			void clearRows();
			
			i64 getSelectedRowIndex();
			
			const ui32& getRowCount() const;
			
			QTableWidgetItem*
			first() const;
			
			QTableWidgetItem*
			last() const;
		
		protected:
			void connectEvents();
			
			void onCursorChanged();
		
		protected:
			void insert(i64 index);
			
			void remove(i64 index);
		
		private:
			ui32 m_rowCount;
			
			QVector<QTableWidgetItem*> m_rows;
		};
	}
}