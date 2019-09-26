/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QLineEdit>

class QMutex;

namespace Dixter
{
	namespace Database
	{
		class TManager;
	}
	namespace OpenTranslate
	{
		class TDictionary;
	}
	
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
			using TDictionaryPtr = std::shared_ptr<OpenTranslate::TDictionary>;
			using TDatabaseManagerPtr = std::shared_ptr<Database::TManager>;
		public:
			explicit TSearchEntry(QWidget* parent, const QString& placeholder = QString("Search"),
								  const QSize& size = QSize(), int margin = 0);
			
			virtual ~TSearchEntry() noexcept;
			
			bool isPlaceholderSet() const;
			
			void search();
		
		protected:
			virtual void init();
		
		private:
			bool m_isPlaceholderSet;
			
			QString m_placeholder;
			
			QMutex* m_mutex;
			
			TDictionaryPtr
			m_dictionary;
			
			TDatabaseManagerPtr
			m_dbManager;
		};
	} // namespace Gui
} // namespace Dixter