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
			Int64 index;
		};
		class TTextList : public QTableWidget
		{
		Q_OBJECT
		public:
			using RowMap = std::map<int, TUString>;
		public:
			TTextList(QWidget* parent = nullptr, int rows = 0, int columns = 0);
			
			~TTextList() override;
			
			void insertColumns(const QStringList& columns, const QString& except = QString());
			
			const UInt32&
			insertRowValues(Int64 index, const std::vector<QString>& values);
			
			void removeRow(const Int64& rowIndex);
			
			bool searchRow(const QString& text, Int64& position, const Int64& start = -1);
			
			void clearRows();
			
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