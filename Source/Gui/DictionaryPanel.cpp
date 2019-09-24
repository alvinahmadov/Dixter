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
#include "OpenTranslate/Dictionary.hpp"

using namespace std;

namespace Dixter
{
	namespace Gui
	{
		namespace StringUtils   = Utilities::Strings;
		namespace AlgoUtils     = Utilities::Algorithms;
		
		template<typename T>
		using TVector = std::vector<T>;
		
		TDictionaryPanel::TDictionaryPanel(QWidget* parent, int width, int height, const QString& name)
				: TPanel(parent, QSize(width, height), name),
				  m_isLanguageSet(false),
				  m_grids(new GridGroup),
				  m_widgets(new WidgetGroup)
		{
			init();
			setObjectName(name);
			connectEvents();
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
		
		std::shared_ptr<TOptionBox>
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
					g_widgetGroup, new TSearchEntry(this, tr("Search...")),
					EWidgetID::SearchControl, true);
			
			__ctrlBox->addWidget(__fromLabel);
			__ctrlBox->addWidget(__pLangBoxL);
			__ctrlBox->addWidget(__pSrchCtrl);
			__ctrlBox->addWidget(__toLabel);
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
		
		void TDictionaryPanel::connectEvents()
		{
			auto __pSrchCtrl = m_widgets->get<TSearchEntry>(g_widgetGroup, EWidgetID::SearchControl);
			connect(
					__pSrchCtrl, SIGNAL(textChanged(
												const QString&)), this, SLOT(onSearch(
																					 const QString&)));
		}
		
		void TDictionaryPanel::setValues()
		{
			auto __langNameList = TVector<TUString>();
			auto __langNameDisplayList = TVector<TUString>();
			auto __langIdList = TVector<TUString>();
			try
			{
				TConfigurationManager::getManager(EConfiguration::XML)
						->getAccessor()
						->getValues(NodeKey::kLangNameNode, __langNameList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangNameDisplayNode, __langNameDisplayList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangIdNode, __langIdList, NodeKey::kLangRoot);
			}
			catch (TException& e)
			{
				printerr(e.what())
			}
			
			auto __languages = TVector<TUString>(__langNameList);
			
			AlgoUtils::foreachCompound<TVector<TUString>>(
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
		{ }
		
		void TDictionaryPanel::onClearButton(void)
		{ }
		
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
				TUString __structureList {};
				TUString __langName {};
				auto __strSel = __pLangBox->currentText();
				try
				{
					__structureList =
							TConfigurationManager::getManager(EConfiguration::XML)
									->getAccessor()
									->getValue(NodeKey::kLangStructureNode, __strSel, NodeKey::kLangRoot);
					__langName =
							TConfigurationManager::getManager(EConfiguration::XML)
									->getAccessor()
									->getValue(NodeKey::kLangNameNode, __strSel, NodeKey::kLangRoot);
				}
				catch (TException& e)
				{
					printerr(e.what())
				}
				Q_UNUSED(__pTextList)
				__pTextList->clear();
				__pTextList->insertColumns(__structureList.asCustom().split(','), "Id");
			}
		}
		
		void TDictionaryPanel::onSearch(dxMAYBE_UNUSED const QString& text)
		{
			/*
			OpenTranslate::TDictionary __dict(m_dbManager, "tables", "original_value");
			auto res = __dict.search(text.toStdString(), "paradigm");
			
			std::ostringstream __oss {};
			uint __index {};
			for (const auto&[__key, __valueVector] : res)
				for (; __index < __valueVector.size(); ++__index)
				{
					__oss << __valueVector.at(__index) << " ";
				}
			
			__oss << g_endLine;
			printl_log(__oss.str())
			*/
		}
		
		QWidget* TDictionaryPanel::getWidget(EWidgetID id)
		{
			return m_widgets->get(id);
		}
	}
}