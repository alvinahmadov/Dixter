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

class QTableWidgetItem;

namespace Dixter
{
	namespace Gui
	{
		struct ItemContainer
		{
			QTableWidgetItem* item;
			Int64 index;
		};
		
		class TTextView : public QTableWidget
		{
		Q_OBJECT
		public:
			using RowMap = std::map<int, TUString>;
			using TViewItem = QTableWidgetItem;
		public:
			TTextView(QWidget* parent = nullptr, int rows = 0, int columns = 0);
			
			~TTextView() override;
			
			void insertColumns(const QStringList& columns, const QString& except = QString());
			
			void setColumnText(int columnIndex, const TString& text);
			
			void setRowText(int rowIndex, int columnIndex, const TString& text);
			
			void removeRow(const Int64& rowIndex);
			
			bool searchRow(const QString& text, Int64& position, const Int64& start = -1);
			
			void clearAll();
			
			Int64 getSelectedRowIndex();
			
			const UInt32& getRowCount() const;
			
			QTableWidgetItem*
			first() const;
			
			QTableWidgetItem*
			last() const;
		
		protected:
			void connectEvents();
			
			void onCursorChanged();
		
		protected:
			void insert(Int64 index);
			
			void remove(Int64 index);
		
		private:
			UInt32 m_rowCount;
			
			QVector<QTableWidgetItem*> m_rows;
		};
	}
}