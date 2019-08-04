/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Utilities.hpp"

#include <QComboBox>

class QWidget;

namespace Dixter
{
	namespace Gui
	{
		class OptionBox : public QComboBox
		{
		Q_OBJECT
		public:
			explicit OptionBox(QWidget* parent,
			                   const QString& placeholder = QString { "Select..." },
			                   const QSize& size = QSize(), bool sort = true);
		
			explicit OptionBox(const QString& placeholder = QString { "Select..." },
			                   const QSize& size = QSize(), bool sort = true);
			
			~OptionBox() dxDECL_OVERRIDE;
			
			void setPlaceholder(const QString& placeholder);
			
			void resetPlaceholder();
			
			void setValues(std::vector<ustring_t>& options, bool sort = false);
			
			void setValues(const QStringList& options);
			
			/// Change current value of src to this one
			void swapCurrent(OptionBox* src);
			
			void sort(bool ascending = true);
			
			int getItemCount() const;
			
			pos_t getPosition(const QString& value);
			
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