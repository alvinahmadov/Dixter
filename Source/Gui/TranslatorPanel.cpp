/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QBoxLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QDebug>

#include "Configuration.hpp"
#include "Gui/TranslatorPanel.hpp"
#include "Gui/TextArea.hpp"
#include "Gui/Button.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/Label.hpp"
#include "OpenTranslate/LanguageBook.hpp"

namespace Dixter
{
	using namespace Utilities::Algorithms;
	
	namespace Gui
	{
		TranslatorPanel::TranslatorPanel(QWidget* parent, int width, int height, const QString& name)
				: Panel(parent, QSize(width, height), name),
		#ifdef USE_SPEECHD
				m_narrator {new vsynth::SpeechDispatcher("dix", "Dixter_conn", "Dixter_user", SPDConnectionMode::SPD_MODE_SINGLE)},
		#endif
		          m_lgs { new OpenTranslate::LanguageBook() },
		          m_grids { new GridGroup },
		          m_widgets { new WidgetGroup }
		{
			init();
			connectEvents();
			setObjectName(name);
		}
		
		TranslatorPanel::~TranslatorPanel()
		{
			#ifdef USE_SPEECHD
			SAFE_RELEASE(m_narrator)
			#endif
			SAFE_RELEASE(m_lgs)
			SAFE_RELEASE(m_grids)
			SAFE_RELEASE(m_widgets)
		}
		
		void TranslatorPanel::show(bool show)
		{
			m_widgets->forEach(&QWidget::setVisible, show);
		}
		
		std::shared_ptr<OptionBox>
		TranslatorPanel::getOptionBox(WidgetID id)
		{
			return std::make_shared<OptionBox>(m_widgets->get<OptionBox>(g_controlGroup, id));
		}
		
		void TranslatorPanel::setValues()
		{
			auto __langNameList = std::vector<ustring_t>();
			auto __langNameDisplayList = std::vector<ustring_t>();
			auto __langIdList = std::vector<ustring_t>();
			auto __voiceList = std::vector<ustring_t>();
			
			try
			{
				auto confMgr = ConfigurationManager::getManager(ConfigurationType::ConfigXml);
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangNameNode, __langNameList);
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangNameDisplayNode, __langNameDisplayList);
				confMgr->getAccessor()->getValues(NodeKey::kLangRoot, NodeKey::kLangIdNode, __langIdList);
				confMgr->getAccessor()->getValues(NodeKey::kVoiceRoot, NodeKey::kVoiceNameNode, __voiceList);
			} catch (std::exception& e)
			{
				printerr(e.what())
			}
			
			foreachCompound(__langNameList, __langIdList,
					[ & ](const ustring_t& langName, const ustring_t& langId)
			{
				m_lgs->insert(langName.asUTF8(), langId.asUTF8());
			});
			
			auto languages = std::vector<ustring_t>(__langNameList);
			auto voices = std::vector<ustring_t>(__voiceList);
			
			foreachCompound<std::vector<ustring_t>>(languages, __langNameDisplayList,
			                                        [ ](ustring_t& langName, const ustring_t& langNameDisplay)
			                                        {
				                                        langName.append(u" / ").append(langNameDisplay);
			                                        });
			
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest)->setValues(languages);
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxEast)->setValues(languages);
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::VoiceBoxT)->setValues(voices);
		}
		
		void TranslatorPanel::init()
		{
			// init widgets
			auto __pvoiceBox = m_widgets->add<OptionBox>(g_widgetGroup,
			                                             new OptionBox(nullptr, tr("Select voice...")), WidgetID::VoiceBoxT);
			auto __pAreaWest = m_widgets->add<TextArea>(g_widgetGroup,
			                                            new TextArea(this, tr("Translation")), WidgetID::TranslatorAreaWest);
			auto __pAreaEast = m_widgets->add<TextArea>(g_widgetGroup,
			                                            new TextArea(this, tr("Translation"), true), WidgetID::TranslatorAreaEast);
			auto __pLangBoxL = m_widgets->add<OptionBox>(g_widgetGroup, new OptionBox(tr("Select language...")), WidgetID::LangboxWest);
			auto __pLangBoxR = m_widgets->add<OptionBox>(g_widgetGroup, new OptionBox(tr("Select language...")), WidgetID::LangboxEast);
			auto __widgetLayoutWest = new QVBoxLayout();
			auto __widgetLayoutEast = new QVBoxLayout();
			auto __widgetGroupWest = new QGroupBox();
			auto __widgetGroupEast = new QGroupBox();
			auto __buttonLayoutL = new QHBoxLayout();
			auto __buttonLayoutR = new QHBoxLayout();
			
			//Buttons
			auto __btnSpeakWest = m_widgets->add<Button>(g_controlGroup,
			                                             new Button(QIcon(":Resources/icons/speak.png")), WidgetID::ButtonSpeakWest);
			auto __btnTranslateWest = m_widgets->add<Button>(g_controlGroup,
			                                                 new Button(QIcon(":Resources/icons/translate.png")), WidgetID::ButtonTranslateWest);
			auto __btnSpeakEast = m_widgets->add<Button>(g_controlGroup,
			                                             new Button(QIcon(":Resources/icons/speak.png")), WidgetID::ButtonSpeakEast);
			auto __btnTranslateEast = m_widgets->add<Button>(g_controlGroup,
			                                                 new Button(QIcon(":Resources/icons/translate.png")), WidgetID::ButtonTranslateEast);
			
			
			__btnSpeakWest->setFixedSize(QSize(150, 50));
			__btnSpeakEast->setFixedSize(QSize(150, 50));
			__btnTranslateWest->setFixedSize(QSize(150, 50));
			__btnTranslateEast->setFixedSize(QSize(150, 50));
			
			// auto btnGroup = new QButtonGroup()
			__buttonLayoutL->addWidget(__btnSpeakWest);
			__buttonLayoutL->addWidget(__btnTranslateWest);
			__buttonLayoutR->addWidget(__btnSpeakEast);
			__buttonLayoutR->addWidget(__btnTranslateEast);
			
			// Set left box
			auto __optBoxL = new QHBoxLayout();
			__optBoxL->addWidget(new Label(tr("From"), __pLangBoxL));
			__optBoxL->addWidget(__pLangBoxL);
			
			__widgetLayoutWest->addLayout(__optBoxL);
			__widgetLayoutWest->addWidget(__pAreaWest);
			__widgetLayoutWest->addLayout(__buttonLayoutL);
			__widgetGroupWest->setLayout(__widgetLayoutWest);
			
			// Set central box
			auto __widgetLayoutCenter = new QVBoxLayout();
			__widgetLayoutCenter->addSpacing(10);
			__widgetLayoutCenter->addWidget(m_widgets->add<Button>(g_controlGroup, new Button(tr("Flip")), WidgetID::ButtonFlip));
			__widgetLayoutCenter->addSpacing(8);
			__widgetLayoutCenter->addWidget(__pvoiceBox);
			__widgetLayoutCenter->addSpacing(10);
			
			// Set right box
			auto __optBoxR = new QHBoxLayout();
			__optBoxR->addWidget(new Label(tr("To"), __pLangBoxR));
			__optBoxR->addWidget(__pLangBoxR);
			
			__widgetLayoutEast->addLayout(__optBoxR);
			__widgetLayoutEast->addWidget(__pAreaEast);
			__widgetLayoutEast->addLayout(__buttonLayoutR);
			__widgetGroupEast->setLayout(__widgetLayoutEast);
			
			// Main grid
			auto __pMainGrid = m_grids->add<QHBoxLayout>(g_layoutGroup, new QHBoxLayout(this),
			                                             WidgetID::Grid);
			__pMainGrid->addWidget(__widgetGroupWest);
			__pMainGrid->addLayout(__widgetLayoutCenter);
			__pMainGrid->addWidget(__widgetGroupEast);
			
			setValues();
			setLayout(__pMainGrid);
		}
		
		std::pair<string_t, string_t>
		TranslatorPanel::getCurrentLanguage()
		{
			string_t __langId { };
			string_t __langName { };
			try
			{
				auto __pBox = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest);
				if (__pBox->isPlaceholderSet())
				{
					return std::pair<string_t, string_t>();
				}
				__langName = __pBox->currentText().toStdString();
				__langId = ConfigurationManager::getManager(ConfigurationType::ConfigXml)
						->getAccessor()
						->getValue(NodeKey::kLangRoot, __langName, NodeKey::kLangNameNode).asUTF8();
			} catch (Exception& e)
			{
				printerr(e.what())
			}
			return std::make_pair(__langId, __langName);
		}
		
		void TranslatorPanel::onBufferChange()
		{ }
		
		void TranslatorPanel::onFlip()
		{
			auto __areaWest = m_widgets->get<TextArea>(g_widgetGroup, WidgetID::TranslatorAreaWest);
			auto __areaEast = m_widgets->get<TextArea>(g_widgetGroup, WidgetID::TranslatorAreaEast);
			auto __optBoxWest = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest);
			auto __optBoxEast = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxEast);
			
			__optBoxWest->swapCurrent(__optBoxEast);
			__areaWest->swapContent(__areaEast);
		}
		
		void TranslatorPanel::onSpeakWest()
		{
			qDebug() << __FUNCTION__;
		}
		
		void TranslatorPanel::onTranslateWest()
		{}
		
		void TranslatorPanel::onSpeakEast()
		{
			qDebug() << __FUNCTION__;
		}
		
		void TranslatorPanel::onTranslateEast()
		{
			qDebug() << __FUNCTION__;
		}
		
		void TranslatorPanel::onLanguageChangeFrom()
		{
			#ifdef USE_SPEECHD
			m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest)->onChanged(choiceEvent);
			ustring_t __langId {};
			auto __pLangBox = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::LangboxWest);
			auto __langName = __pLangBox->GetStringSelection();
			try
			{
				auto confMgr = ConfigurationManager::getManager();
				__langId = confMgr->xmlManager()->getValue(NodeKey::kLangRoot, NodeKey::kLangNameNode, __langName);
			} catch (...) {}
			m_narrator->setLanguage(TargetMode::kDirectionAll, __langId);
			#endif
		}
		
		void TranslatorPanel::onLanguageChangeTo()
		{
		}
		
		void TranslatorPanel::onTextAreaEdit()
		{
		}
		
		void TranslatorPanel::onVoiceChange()
		{
			#ifdef USE_SPEECHD
			QString __voiceId {};
	auto __voiceName = m_widgets->get<OptionBox>(g_widgetGroup, WidgetID::VoiceBoxT)->GetStringSelection();

	try
	{
		auto confMgr = ConfigurationManager::getManager();
		__voiceId = confMgr->xmlManager()->getValue(NodeKey::kVoiceRoot, NodeKey::kVoiceNameNode, __voiceName);
		#ifdef DIXTER_DEBUG
		SAFE_RELEASE(confMgr)
		#endif
	} catch (...) {}


	if (!__voiceId.empty())
		m_narrator->setSynthesisVoice(TargetMode::kSingle, __voiceId);
	PUTL(__voiceName)
			#endif
		}
		
		void TranslatorPanel::connectEvents()
		{
			connect(m_widgets->get<Button>(g_controlGroup, WidgetID::ButtonFlip),
			        SIGNAL(clicked()), SLOT(onFlip()));
			connect(m_widgets->get<Button>(g_controlGroup, WidgetID::ButtonSpeakWest),
			        SIGNAL(clicked()), SLOT(onSpeakWest()));
			connect(m_widgets->get<Button>(g_controlGroup, WidgetID::ButtonTranslateWest),
			        SIGNAL(clicked()), SLOT(onTranslateWest()));
			
			connect(m_widgets->get<Button>(g_controlGroup, WidgetID::ButtonSpeakEast),
			        SIGNAL(clicked()), SLOT(onSpeakEast()));
			connect(m_widgets->get<Button>(g_controlGroup, WidgetID::ButtonTranslateEast),
			        SIGNAL(clicked()), SLOT(onTranslateEast()));
		}
		
		QWidget* TranslatorPanel::getWidget(WidgetID id)
		{
			return m_widgets->get(id);
		}
	}
}