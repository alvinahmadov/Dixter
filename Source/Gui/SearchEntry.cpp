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
		
		void TSearchEntry::search()
		{
			const auto __text = text();
			
			if (__text.isEmpty())
				return;
			
			if (not m_dbManager)
			{
				print_log("Database connection isn't established.")
				return;
			}
			
			QMutexLocker __ml(m_mutex);
			const TString __paradigm("paradigm");
			const auto& __fetchData = m_dictionary->search(
					__text.toStdString(),
					__paradigm, true);
			for (const auto&[__key, __valueVector] : __fetchData)
				printl_log("Key: " << __key <<
								   " ->  " << Utilities::Strings::toString(__valueVector))
			printeol
		}
	} // namespace Gui
} // namespace Dixter