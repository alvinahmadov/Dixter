/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <regex>

#include "Macros.hpp"
#include "JoinThread.hpp"
#include "Constants.hpp"
#include "Exception.hpp"
#include "Configuration.hpp"
#include "OpenTranslate/Dictionary.hpp"
#include "Database/Manager.hpp"
#include "Gui/TextView.hpp"
#include "Gui/SearchEntry.hpp"

using TSearchResult 	= Dixter::OpenTranslate::TDictionary::TSearchResult;
using FHideCondition 	= std::function<bool(Dixter::TStringView)>;
using FRenameCondition 	= std::function<void(Dixter::TString&)>;

namespace Dixter
{
	namespace Gui
	{
		void populateTextView(const TSearchResult& fetchedData, TTextView* textView,
							  FHideCondition& hideIf, FRenameCondition& renameIfMatches)
		{
			textView->clearAll();
			for (const auto& __data : fetchedData)
			{
				auto __key = __data.first;
				const auto& __values = __data.second;
				const int __valuesLen = static_cast<int>(__values.size());
				
				if (hideIf(__key))
					continue;
				
				renameIfMatches(__key);
				textView->insertColumn(0);
				textView->setColumnText(0, __key);
				textView->setRowCount(__values.size());
				
				if (__valuesLen == 1)
					textView->setRowText(0, 0, __values.front());
				else {
					for (int __i = 0; __i < __valuesLen; ++__i)
						textView->setRowText(__i, 0, __values.at(__i));
				}
			}
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
				auto __confManIni = getIniManager({ g_guiConfigPath });
				m_dbManager.reset(
						new Database::TManager(
								__confManIni->accessor()->getValue(NodeKey::kDatabaseHostNode).asUTF8(),
								__confManIni->accessor()->getValue(NodeKey::kDatabaseUserNode).asUTF8(),
								__confManIni->accessor()->getValue(NodeKey::kDatabasePassNode).asUTF8()));
				
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
			
			m_dbManager->selectDatabase(database);

			const auto& __fetchedData = m_dictionary->lookFor(__text, keyColumn, true);
			
			if (not __fetchedData.size())
			{
				textView->clearAll();
				printl_log("No such a word.");
				return;
			}
			FHideCondition __hideIf =
					[](TStringView column) { return (column == "id" or column == "word"); };
			
			FRenameCondition __renameIfMatches =
					[] (TString& column)
			{
				std::smatch __match;
				std::regex __categoryRegex("category_\\d*", std::regex::icase);
				if (std::regex_search(column, __match, __categoryRegex))
					column = std::regex_replace(column, __categoryRegex, "Category");
			};
			
			populateTextView(__fetchedData, textView, __hideIf, __renameIfMatches);
		}
	} // namespace Gui
} // namespace Dixter