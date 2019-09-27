/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QMutex>

#include "Configuration.hpp"
#include "Constants.hpp"
#include "Macros.hpp"
#include "Exception.hpp"
#include "OpenTranslate/Dictionary.hpp"
#include "Database/Manager.hpp"
#include "Gui/TextView.hpp"
#include "Gui/SearchEntry.hpp"

namespace Dixter
{
	namespace Gui
	{
		TSearchEntry::TSearchEntry(QWidget* parent, const QString& placeholder,
								   const QSize& size, int margin)
				: QLineEdit(parent),
				  m_isPlaceholderSet(),
				  m_placeholder(placeholder),
				  m_mutex(new QMutex),
				  m_dbManager(nullptr)
		{
			init();
			setTextMargins(margin, margin, margin, margin);
			setPlaceholderText(placeholder);
			if (size.width() > 0 and size.height() > 0)
				setMinimumSize(size);
		}
		TSearchEntry::~TSearchEntry() noexcept
		{
			delete m_mutex;
		}
		
		void TSearchEntry::init()
		{
			try
			{
				auto __confManIni = getIniManager({ g_guiConfigPath })->accessor();
				m_dbManager.reset(
						new Database::TManager(
								__confManIni->getValue(NodeKey::kDatabaseHostNode).asUTF8(),
								__confManIni->getValue(NodeKey::kDatabaseUserNode).asUTF8(),
								__confManIni->getValue(NodeKey::kDatabasePassNode).asUTF8()));
				
				m_dbManager->selectDatabase("dixterdb_NO");
				m_dictionary = dxMAKE_SHARED(OpenTranslate::TDictionary, m_dbManager);
			}
			catch (TSQLException& e)
			{
				printerr(e.getMessage())
			}
		}
		
		bool TSearchEntry::isPlaceholderSet() const
		{
			return m_isPlaceholderSet;
		}
		
		void populate(const OpenTranslate::TDictionary::TSearchResult& fetchedData,
					  TTextView* textView)
		{
			// Amsterdam  5
			// gammelordføreren 16
			textView->clearAll();
			
			if (not textView->isEnabled())
				textView->setEnabled(true);
			
			for (const auto&[__key, __valueVector] : fetchedData)
			{
				int __columnIndex = -1;
				
				textView->insertColumn(++__columnIndex);
				textView->setColumnText(__columnIndex, __key);
				textView->setRowCount(__valueVector.size());
				
				if (__valueVector.size() == 1)
					textView->setRowText(0, __columnIndex, __valueVector.back());
				else
				{
					for (const auto& __value : __valueVector)
					{
						int __rowIndex { 0 };
						textView->setRowText(++__rowIndex, __columnIndex, __value);
					}
				}
			}
		}
		
		void TSearchEntry::search(TTextView* textView)
		{
			dxTIMER_START
			const auto __text = text();
			
			if (__text.isEmpty())
				return;
			
			if (not m_dbManager)
			{
				print_log("Database connection isn't established.")
				return;
			}
			
			QMutexLocker __ml(m_mutex);
			
			const auto& __fetchedData =
					m_dictionary->lookFor(__text.toStdString(), "paradigm");
			
			if (not __fetchedData.size())
			{
				printl_log("No such a word.");
				return;
			}
			
			populate(__fetchedData, textView);
		}
	} // namespace Gui
} // namespace Dixter