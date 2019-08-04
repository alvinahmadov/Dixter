/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "Gui/DictionaryPanel.hpp"
#include "Gui/TextList.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/Label.hpp"
#include "OpenTranslate/Alphabet.hpp"

using namespace std;

namespace Dixter
{
	namespace Gui
	{
		using namespace Utilities::Strings;
		using namespace Utilities::Algorithms;
		
		DictionaryPanel::DictionaryPanel(QWidget* parent, int width, int height, const QString& name)
				: Panel(parent, QSize(width, height), name),
				  m_isLanguageSet { false },
				  m_grids { new GridGroup },
				  m_widgets { new WidgetGroup },
				  m_languages { new OpenTranslate::LanguageBook() }
		{
			init();
			setObjectName(name);
		}
		
		DictionaryPanel::~DictionaryPanel()
		{
			SAFE_RELEASE(m_grids)
			SAFE_RELEASE(m_widgets)
			SAFE_RELEASE(m_languages)
		}
		
		void DictionaryPanel::show(bool show)
		{
			m_widgets->forEach(&QWidget::setVisible, show);
		}
		
		std::shared_ptr<OptionBox>
		DictionaryPanel::getOptionBox(WidgetID widgetID)
		{
			return std::make_shared<OptionBox>(m_widgets->get<OptionBox>(g_widgetGroup, widgetID));
		}
		
		void DictionaryPanel::init()
		{
			auto __pMainGrid = m_grids->add<QVBoxLayout>(g_layoutGroup, new QVBoxLayout(this),
			                                             WidgetID::Grid);
			auto __pTextList = m_widgets->add<TextList>(g_widgetGroup, new TextList(),
			                                            WidgetID::TextView);
			auto __pWordBox = m_grids->add<QVBoxLayout>(g_layoutGroup, new QVBoxLayout(),
			                                            WidgetID::WordBox);
			auto __pLangBoxL = m_widgets->add<OptionBox>(g_widgetGroup, new OptionBox(nullptr, QString("Select language...")),
			                                             WidgetID::LangboxWest);
			auto __pLangBoxR = m_widgets->add<OptionBox>(g_widgetGroup, new OptionBox(nullptr, QString("Select language...")),
			                                             WidgetID::LangboxEast);
			auto __controlGroup = new QGroupBox();
			auto __ctrlBox = new QHBoxLayout();
			auto fromLabel = new Label(tr("From"), __pLangBoxL);
			auto toLabel = new Label(tr("To"), __pLangBoxR);
			
			// Search Entry for words in Database
			auto __pSrchCtrl = m_widgets->add<SearchEntry>(g_widgetGroup, new SearchEntry(this, tr("Search...")),
			                                               WidgetID::SearchControl, true);
			
			__ctrlBox->addWidget(fromLabel);
			__ctrlBox->addWidget(__pLangBoxL);
			__ctrlBox->addWidget(__pSrchCtrl);
			__ctrlBox->addWidget(toLabel);
			__ctrlBox->addWidget(__pLangBoxR);
			
			__controlGroup->setLayout(__ctrlBox);
			
			__pTextList->setEnabled(false);
			__pWordBox->addWidget(__pTextList);
			
			// Populate main grid with items
			__pMainGrid->setMargin(10);
			__pMainGrid->addWidget(__controlGroup);
			// __pMainGrid->addWidget(__pSrchCtrl);
			__pMainGrid->addLayout(__pWordBox);
			
			__ctrlBox->setSizeConstraint(QLayout::SizeConstraint::SetNoConstraint);
			setValues();
			
			setLayout(__pMainGrid);
		}
		
		void DictionaryPanel::connectEvents()
		{
		}
		
		void DictionaryPanel::setValues()
		{
			
			auto __langNameList = vector<ustring_t>();
			auto __langNameDisplayList = std::vector<ustring_t>();
			auto __langIdList = vector<ustring_t>();
			try
			{
				auto confMgr = ConfigurationManager::getManager(ConfigurationType::kConfigXml);
				
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangNameNode, __langNameList);
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangNameDisplayNode, __langNameDisplayList);
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangIdNode, __langIdList);
			} catch (Exception& e)
			{
				printerr(e.what())
			}
			
			foreachCompound(__langNameList, __langIdList,
			                [ & ](const ustring_t& langName, const ustring_t& langId)
			                {
				                m_languages->insert(langName.asUTF8(), langId.asUTF8());
			                });
			
			auto __languages = std::vector<ustring_t>(__langNameList);
			
			foreachCompound<std::vector<ustring_t>>(__languages, __langNameDisplayList,
			                                        [ ](ustring_t& langName, const ustring_t& langNameDisplay)
			                                        {
				                                        langName.append(u" / ");
				                                        langName.append(langNameDisplay);
			                                        });
			
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest)->setValues(__languages);
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxEast)->setValues(__languages);
		}
		
		void DictionaryPanel::onCopyButton(void* event)
		{
			Q_UNUSED(event)
		}
		
		void DictionaryPanel::onClearButton(void* event)
		{
			Q_UNUSED(event)
		}
		
		void DictionaryPanel::onSearch(void* event)
		{
			Q_UNUSED(event)
		}
		
		void DictionaryPanel::onLanguageChange(void* event)
		{
			Q_UNUSED(event)
			OptionBox* __pLangBox = nullptr;
			try
			{
				__pLangBox = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest);
			} catch (Exception& e)
			{
				printerr(e.what())
				return;
			}
			
			// __pLangBox->onChanged(event);
			auto __pTextList = m_widgets->get<TextList>(g_widgetGroup, WidgetID::TextView);
			if (!__pLangBox->isPlaceholderSet())
			{
				ustring_t __structureList { };
				ustring_t __langName { };
				auto __strSel = __pLangBox->currentText();
				try
				{
					auto confMgr = ConfigurationManager::getManager(ConfigurationType::kConfigXml);
					__structureList = confMgr->getAccessor()->getValue(NodeKey::kLangRoot, __strSel, NodeKey::kLangStructureNode);
					__langName = confMgr->getAccessor()->getValue(NodeKey::kLangRoot, __strSel, NodeKey::kLangNameNode);
				} catch (Exception& e)
				{
					printerr(e.what())
				}
				Q_UNUSED(__pTextList)
				__pTextList->clear();
				__pTextList->insertColumns(__structureList.asCustom().split(','), "Id");
			}
		}
		
		QWidget* DictionaryPanel::getWidget(WidgetID id)
		{
			return m_widgets->get(id);
		}
	}
}