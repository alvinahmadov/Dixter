/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QComboBox>

class QWidget;

namespace Dixter
{
	namespace Gui
	{
		class TOptionBox : public QComboBox
		{
		Q_OBJECT
		public:
			TOptionBox(QWidget* parent,
					   const QString& placeholder = QString("Select..." ),
					   const QSize& size = QSize(), bool sort = true);
		
			TOptionBox(const QString& placeholder = QString("Select..."),
					   const QSize& size = QSize(), bool sort = true);
			
			~TOptionBox() override = default;
			
			void setPlaceholder(const QString& placeholder);
			
			void resetPlaceholder();
			
			void setValues(std::vector<TUString>& options, bool sort = false);
			
			void setValues(const QStringList& options);
			
			void swapCurrent(TOptionBox* src);
			
			void sort(bool ascending = true);
			
			Int32 getItemCount() const;
			
			Int32 getPosition(const QString& value);
			
			bool isPlaceholderSet() const;
		
		private slots:
			void onChanged(int i);
		
		protected:
			
			void connectEvents();
		
		private:
			bool m_isPlaceholderSet;
			
			bool m_sort;
			
			QString m_placeHolder;
		};
	}
}