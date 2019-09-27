/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Macros.hpp"
#include "Constants.hpp"
#include "Exception.hpp"
#include "Configuration.hpp"
#include "OpenTranslate/Dictionary.hpp"
#include "Database/Manager.hpp"
#include "Gui/TextView.hpp"
#include "Gui/SearchEntry.hpp"

using TSearchResult = Dixter::OpenTranslate::TDictionary::TSearchResult;

namespace Dixter
{
	namespace Gui
	{
		void populateTextView(const TSearchResult& fetchedData, TTextView* textView,
							  std::set<int> columnsToHide)
		{
			dxTIMER_START
			textView->clearAll();
			
			if (not textView->isSortingEnabled())
				textView->setSortingEnabled(true);
			
			for (const auto& __data : fetchedData)
			{
				int __colIdx = 0;
				const auto& __key = __data.first;
				const auto& __values = __data.second;
				
				const int __valuesLen = static_cast<int>(__values.size());
				
				textView->insertColumn(__colIdx);
				textView->setColumnText(__colIdx, __key);
				textView->setRowCount(__values.size());
				
				if (__valuesLen == 1)
					textView->setRowText(0, __colIdx, __values.front());
				else
					for (int __i = 0; __i < __valuesLen; ++__i)
						textView->setRowText(__i, __colIdx, __values.at(__i));
				++__colIdx;
			}
			
			for (int __hideIndex : columnsToHide)
				textView->hideColumn(__hideIndex);
			
			printfm("Read %i data.", textView->rowCount())
		}
		
		TSearchEntry::TSearchEntry(QWidget* parent, const QString& placeholder,
								   const QSize& size, int margin)
				: QLineEdit(parent),
				  m_isPlaceholderSet(),
				  m_placeholder(placeholder),
				  m_dbManager(nullptr)
		{
			init();
			setTextMargins(margin, margin, margin, margin);
			setPlaceholderText(placeholder);
			if (size.width() > 0 and size.height() > 0)
				setMinimumSize(size);
		}
		
		TSearchEntry::~TSearchEntry() noexcept
		{ }
		
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
		
		void TSearchEntry::search(const TString& database, const TString& keyColumn,
								  TTextView* textView)
		{
			auto __text = text().toStdString();
			
			m_dbManager->selectDatabase(database);
			
			if (__text.empty())
			{
				textView->clearAll();
				return;
			}
			
			if (not m_dbManager)
			{
				print_log("Database connection isn't established.")
				return;
			}
			
			auto& __fetchedData = m_dictionary->lookFor(__text, keyColumn, true);
			
			if (not __fetchedData.size())
			{
				textView->clearAll();
				printl_log("No such a word.");
				return;
			}
			
			populateTextView(__fetchedData, textView, { 0, 1 });
		}
	} // namespace Gui
} // namespace Dixter