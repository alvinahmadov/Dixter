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
		class TSearchEntry : public QLineEdit
		{
		Q_OBJECT
		public:
			explicit TSearchEntry(QWidget* parent, const QString& placeholder = QString("Search"),
								  const QSize& size = QSize(), int margin = 0);
			
			virtual ~TSearchEntry() noexcept override = default;
			
			bool isPlaceholderSet() const;
			
			// public slots:
			// 	void onEnter(const QString& text);
		
		protected:
			void connectEvents();
		
		private:
			bool m_isPlaceholderSet;
			
			QString m_placeholder;
		};
	}
}