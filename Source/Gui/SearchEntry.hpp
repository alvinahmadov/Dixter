/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Macros.hpp"

#include <QLineEdit>

namespace Dixter
{
	namespace Gui
	{
		/**
		 * \brief SearchEntry is a class that responsible for
		 * searching a word in the database
		 * */
		class SearchEntry : public QLineEdit
		{
		Q_OBJECT
		public:
			explicit SearchEntry(QWidget* parent, const QString& placeholder = "Search",
			                     const QSize& size = QSize(), int margin = 0);
			
			~SearchEntry() dxDECL_OVERRIDE;
			
			bool isPlaceholderSet() const;
		
		protected slots:
			
			void onEnter(const QString& text);
		
		protected:
			void connectEvents();
		
		private:
			bool m_isPlaceholderSet;
			
			QString m_placeholder;
		};
	}
}