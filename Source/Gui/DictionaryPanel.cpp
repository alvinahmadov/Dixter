/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QBoxLayout>
#include <QGroupBox>
#include <QAction>

#include "Group.hpp"
#include "Configuration.hpp"
#include "Constants.hpp"
#include "Database/Manager.hpp"
#include "Gui/DictionaryPanel.hpp"
#include "Gui/Label.hpp"
#include "Gui/Button.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/TextView.hpp"
#include "Gui/SearchEntry.hpp"
#include "OpenTranslate/Dictionary.hpp"

using namespace std;

namespace Dixter
{
	namespace Gui
	{
		namespace StringUtils   = Utilities::Strings;
		namespace AlgoUtils     = Utilities::Algorithms;
		
		TDictionaryPanel::TDictionaryPanel(QWidget* parent, int width, int height, const QString& name)
				: APanel(parent, QSize(width, height)),
				  m_isLanguageSet(false),
				  m_grids(new GridGroup),
				  m_widgets(new WidgetGroup)
		{
			init();
			connectEvents();
			name.isEmpty() ? setObjectName(g_dictionaryName)
						   : setObjectName(name);
		}
		
		TDictionaryPanel::~TDictionaryPanel()
		{
			delete m_grids;
			delete m_widgets;
		}
		
		void TDictionaryPanel::show(bool show)
		{
			m_widgets->forEach(&QWidget::setVisible, show);
		}
		
		APanel::TOptionBoxPtr
		TDictionaryPanel::getOptionBox(EWidgetID widgetID)
		{
			return dxMAKE_SHARED(TOptionBox, m_widgets->get<TOptionBox>(g_widgetGroup, widgetID));
		}
		
		void TDictionaryPanel::init()
		{
			auto __mainGrid = m_grids
					->add<QVBoxLayout>(g_layoutGroup,
									   new QVBoxLayout(this),
									   EWidgetID::Grid);
			auto __textView = m_widgets
					->add<TTextView>(g_widgetGroup,
									 new TTextView(this, true),
									 EWidgetID::TextView);
			auto __langBoxWest = m_widgets
					->add<TOptionBox>(g_widgetGroup,
									  new TOptionBox(nullptr,
													 QString("Select language...")),
									  EWidgetID::LangboxWest);
			auto __langBoxEast = m_widgets
					->add<TOptionBox>(g_widgetGroup,
									  new TOptionBox(nullptr,
													 QString("Select language...")),
									  EWidgetID::LangboxEast);
			
			auto __controlGroup = new QGroupBox;
			auto __controlBox = new QHBoxLayout;
			auto __labelFrom = new TLabel(tr("From"), __langBoxWest);
			auto __labelTo = new TLabel(tr("To"), __langBoxEast);
			
			// Search entry for words in a Database
			auto __searchEntry = m_widgets
					->add<TSearchEntry>(g_widgetGroup,
										new TSearchEntry(this, tr("Word...")),
										EWidgetID::SearchControl, true);
			auto __searchButton = m_widgets
					->add<TButton>(g_widgetGroup,
								   new TButton(this, tr("Search")),
								   EWidgetID::SearchButton, true);
			
			__controlBox->addWidget(__labelFrom);
			__controlBox->addWidget(__langBoxWest);
			__controlBox->addWidget(__searchEntry);
			__controlBox->addWidget(__searchButton);
			__controlBox->addWidget(__labelTo);
			__controlBox->addWidget(__langBoxEast);
			
			__controlGroup->setLayout(__controlBox);
			
			// Populate main grid with items
			__mainGrid->addWidget(__controlGroup);
			__mainGrid->addWidget(__textView);
			__textView->setAlternatingRowColors(true);
			__textView->setSortingEnabled(false);
			
			setValues();
			setLayout(__mainGrid);
			
		}
		
		void TDictionaryPanel::connectEvents()
		{
			connect(m_widgets->get<TButton>(EWidgetID::SearchButton), SIGNAL(clicked()),
					this, SLOT(onSearch()));
		}
		
		void TDictionaryPanel::setValues()
		{
			std::vector<TUString> __languageNames {}, __languageDisplayNames {};
			
			try
			{
				getXmlManager({ g_langConfigPath, g_voiceConfigPath })
						->accessor()
						->getValues(NodeKey::kLangNameNode, __languageNames, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangNameDisplayNode, __languageDisplayNames, NodeKey::kLangRoot);
			}
			catch (TException& e)
			{ printerr(e.what()); }
			
			AlgoUtils::foreachCompound<std::vector<TUString>>(
					__languageNames, __languageDisplayNames,
					[](TUString& langName, const TUString& langNameDisplay)
					{ langName.append(u" / ").append(langNameDisplay); }
															 );
			
			m_widgets->get<TOptionBox>(EWidgetID::LangboxWest)->setValues(__languageNames);
			m_widgets->get<TOptionBox>(EWidgetID::LangboxEast)->setValues(__languageNames);
		}
		
		void TDictionaryPanel::onCopyButton(void)
		{
			auto __searchControl = m_widgets->get<TSearchEntry>(EWidgetID::ButtonLayout);
			__searchControl->selectAll();
			__searchControl->copy();
		}
		
		void TDictionaryPanel::onClearButton(void)
		{
			auto __searchControl = m_widgets->get<TSearchEntry>(EWidgetID::ButtonLayout);
			__searchControl->selectAll();
			__searchControl->cut();
		}
		
		void TDictionaryPanel::onLanguageChange(void)
		{
			auto __languagesBox = m_widgets->get<TOptionBox>(EWidgetID::LangboxWest);
			auto __textView = m_widgets->get<TTextView>(EWidgetID::TextView);
			
			if (__languagesBox and not __languagesBox->isPlaceholderSet())
			{
				TUString __structures;
				TUString __languageName;
				auto __strSel = __languagesBox->currentText();
				try
				{
					__structures = getXmlManager()
							->accessor()
							->getValue(NodeKey::kLangStructureNode,
									   __strSel, NodeKey::kLangRoot);
					
					__languageName = getXmlManager()
							->accessor()
							->getValue(NodeKey::kLangNameNode,
									   __strSel, NodeKey::kLangRoot);
					
					__textView->clear();
					__textView->insertColumns(__structures.asCustom().split(','), "Id");
				} catch (TException& e)
				{ printerr(e.what()); }
			}
		}
		
		void TDictionaryPanel::onSearch() noexcept
		{
			try
			{
				auto __textView = m_widgets->get<TTextView>(EWidgetID::TextView);
				m_widgets->get<TSearchEntry>(EWidgetID::SearchControl)
						 ->search("dixterdb_NO", "paradigm", __textView);
			} catch (TException& e)
			{ printerr(e.getMessage()); }
		}
		
		QWidget* TDictionaryPanel::getWidget(EWidgetID id)
		{
			return m_widgets->get(id);
		}
	} // namespace Gui
} // namespace Dixter