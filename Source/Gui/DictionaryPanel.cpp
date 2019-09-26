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
#include "Constants.hpp"
#include "Database/Manager.hpp"
#include "Gui/DictionaryPanel.hpp"
#include "Gui/Label.hpp"
#include "Gui/Button.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/TextList.hpp"
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
			auto __pMainGrid = m_grids->add<QVBoxLayout>(
					g_layoutGroup, new QVBoxLayout(this), EWidgetID::Grid);
			auto __pTextList = m_widgets->add<TTextList>(
					g_widgetGroup, new TTextList(), EWidgetID::TextView);
			auto __pWordBox = m_grids->add<QVBoxLayout>(
					g_layoutGroup, new QVBoxLayout(), EWidgetID::WordBox);
			auto __pLangBoxL = m_widgets->add<TOptionBox>(
					g_widgetGroup,
					new TOptionBox(nullptr, QString("Select language...")),
					EWidgetID::LangboxWest);
			auto __pLangBoxR = m_widgets->add<TOptionBox>(
					g_widgetGroup,
					new TOptionBox(nullptr, QString("Select language...")),
					EWidgetID::LangboxEast);
			auto __controlGroup = new QGroupBox();
			auto __ctrlBox = new QHBoxLayout();
			auto __fromLabel = new TLabel(tr("From"), __pLangBoxL);
			auto __toLabel = new TLabel(tr("To"), __pLangBoxR);
			
			// Search entry for words in a Database
			auto __pSrchCtrl = m_widgets->add<TSearchEntry>(
					g_widgetGroup, new TSearchEntry(this, tr("Word...")),
					EWidgetID::SearchControl, true);
			
			auto __searchBtn = new TButton("Search");
			connect(__searchBtn, &TButton::clicked, this, &TDictionaryPanel::onSearch);
			
			__ctrlBox->addWidget(__fromLabel);
			__ctrlBox->addWidget(__pLangBoxL);
			__ctrlBox->addWidget(__pSrchCtrl);
			__ctrlBox->addWidget(__searchBtn);
			__ctrlBox->addWidget(__toLabel);
			__ctrlBox->addWidget(__pLangBoxR);
			
			__controlGroup->setLayout(__ctrlBox);
			
			__pTextList->setEnabled(false);
			__pWordBox->addWidget(__pTextList);
			
			// Populate main grid with items
			__pMainGrid->addWidget(__controlGroup);
			__pMainGrid->addLayout(__pWordBox);
			
			setValues();
			setLayout(__pMainGrid);
		}
		
		void TDictionaryPanel::connectEvents()
		{ }
		
		void TDictionaryPanel::setValues()
		{
			auto __langNameList = std::vector<TUString>();
			auto __langNameDisplayList = std::vector<TUString>();
			auto __langIdList = std::vector<TUString>();
			try
			{
				getXmlManager({g_langConfigPath, g_voiceConfigPath})
						->accessor()
						->getValues(NodeKey::kLangNameNode, __langNameList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangNameDisplayNode, __langNameDisplayList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangIdNode, __langIdList, NodeKey::kLangRoot);
			}
			catch (TException& e)
			{
				printerr(e.what())
			}
			
			auto __languages = std::vector<TUString>(__langNameList);
			
			AlgoUtils::foreachCompound<std::vector<TUString>>(
					__languages, __langNameDisplayList,
					[](TUString& langName, const TUString& langNameDisplay)
					{
						langName.append(u" / ");
						langName.append(langNameDisplay);
					});
			
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest)->setValues(__languages);
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxEast)->setValues(__languages);
		}
		
		void TDictionaryPanel::onCopyButton(void)
		{
			auto __pSrchCtrl = m_widgets->get<TSearchEntry>(g_controlGroup, EWidgetID::ButtonLayout);
			__pSrchCtrl->selectAll();
			__pSrchCtrl->copy();
		}
		
		void TDictionaryPanel::onClearButton(void)
		{
			auto __pSrchCtrl = m_widgets->get<TSearchEntry>(g_controlGroup, EWidgetID::ButtonLayout);
			__pSrchCtrl->selectAll();
			__pSrchCtrl->cut();
		}
		
		void TDictionaryPanel::onLanguageChange(void)
		{
			TOptionBox* __pLangBox = nullptr;
			try
			{
				__pLangBox = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
			}
			catch (TException& e)
			{
				printerr(e.what())
				return;
			}
			
			// __pLangBox->onChanged(event);
			auto __pTextList = m_widgets->get<TTextList>(g_widgetGroup, EWidgetID::TextView);
			if (not __pLangBox->isPlaceholderSet())
			{
				TUString __structureList;
				TUString __langName;
				auto __strSel = __pLangBox->currentText();
				try
				{
					__structureList =
							getXmlManager()->accessor()
										   ->getValue(
												   NodeKey::kLangStructureNode,
												   __strSel, NodeKey::kLangRoot);
					__langName =
							getXmlManager()->accessor()
										   ->getValue(
												   NodeKey::kLangNameNode,
												   __strSel, NodeKey::kLangRoot);
					__pTextList->clear();
					__pTextList->insertColumns(__structureList.asCustom().split(','), "Id");
				}
				catch (TException& e)
				{
					printerr(e.what())
				}
			}
		}
		
		void TDictionaryPanel::onSearch() noexcept
		{
			try
			{
				m_widgets->get<TSearchEntry>(g_widgetGroup,EWidgetID::SearchControl)->search();
			}
			catch (TException& e)
			{
				printerr(e.getMessage())
			}
		}
		
		QWidget* TDictionaryPanel::getWidget(EWidgetID id)
		{
			return m_widgets->get(id);
		}
	} // namespace Gui
} // namespace Dixter