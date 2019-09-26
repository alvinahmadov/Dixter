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

#include "Configuration.hpp"
#include "Constants.hpp"
#include "Group.hpp"
#include "Utilities.hpp"
#include "Gui/TranslatorPanel.hpp"
#include "Gui/TextEdit.hpp"
#include "Gui/Label.hpp"
#include "Gui/Button.hpp"
#include "Gui/OptionBox.hpp"

namespace Dixter
{
	namespace AlgoUtils = Utilities::Algorithms;
	
	namespace Gui
	{
		TTranslatorPanel::TTranslatorPanel(QWidget* parent, int width, int height, const QString& name)
				: APanel(parent, QSize(width, height)),
				  m_grids(new GridGroup),
				  m_widgets(new WidgetGroup)
		#ifdef USE_SPEECHD
				 ,m_narrator(new VSynth::SpeechDispatcher("dix", "Dixter_conn", "Dixter_user", SPDConnectionMode::SPD_MODE_SINGLE))
		#endif
		{
			init();
			connectEvents();
			name.isEmpty() ? setObjectName(g_translatorName)
						   : setObjectName(name);
		}
		
		TTranslatorPanel::~TTranslatorPanel()
		{
			#ifdef USE_SPEECHD
			delete m_narrator;
			#endif
			delete m_grids;
			delete m_widgets;
		}
		
		void TTranslatorPanel::show(bool show)
		{
			m_widgets->forEach(&QWidget::setVisible, show);
		}
		
		APanel::TOptionBoxPtr
		TTranslatorPanel::getOptionBox(EWidgetID id)
		{
			return dxMAKE_SHARED(TOptionBox, m_widgets->get<TOptionBox>(g_controlGroup, id));
		}
		
		void TTranslatorPanel::setValues()
		{
			auto __langNameList = std::vector<TUString>();
			auto __langNameDisplayList = std::vector<TUString>();
			auto __langIdList = std::vector<TUString>();
			auto __voiceList = std::vector<TUString>();
			
			try
			{
				TConfigurationManager::getManager(EConfiguration::XML)
						->accessor()
						->getValues(NodeKey::kLangNameNode, __langNameList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangNameDisplayNode, __langNameDisplayList, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangIdNode, __langIdList, NodeKey::kLangRoot)
						->getValues(NodeKey::kVoiceNameNode, __voiceList, NodeKey::kVoiceRoot);
			}
			catch (std::exception& e)
			{
				printerr(e.what())
			}
			
			auto languages = std::vector<TUString>(__langNameList);
			auto voices = std::vector<TUString>(__voiceList);
			
			AlgoUtils::foreachCompound<std::vector<TUString>>(
					languages, __langNameDisplayList,
					[](TUString& langName, const TUString& langNameDisplay)
					{
						langName.append(u" / ").append(langNameDisplay);
					});
			
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest)->setValues(languages);
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxEast)->setValues(languages);
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::VoiceBoxT)->setValues(voices);
		}
		
		void TTranslatorPanel::init()
		{
			// init widgets
			const QSize __buttonSize = QSize(150, 50);
			auto __pvoiceBox = m_widgets->add<TOptionBox>(
					g_widgetGroup,
					new TOptionBox(nullptr, tr("Select voice...")), EWidgetID::VoiceBoxT);
			auto __pAreaWest = m_widgets->add<TTextEdit>(
					g_widgetGroup,
					new TTextEdit(this, tr("Translation")), EWidgetID::TranslatorAreaWest);
			auto __pAreaEast = m_widgets->add<TTextEdit>(g_widgetGroup,
					new TTextEdit(this, tr("Translation"), true), EWidgetID::TranslatorAreaEast);
			auto __pLangBoxL = m_widgets->add<TOptionBox>(g_widgetGroup,
					new TOptionBox(tr("Select language...")), EWidgetID::LangboxWest);
			auto __pLangBoxR = m_widgets->add<TOptionBox>(g_widgetGroup,
					new TOptionBox(tr("Select language...")), EWidgetID::LangboxEast);
			
			auto __widgetLayoutWest = new QVBoxLayout();
			auto __widgetLayoutEast = new QVBoxLayout();
			auto __widgetGroupWest = new QGroupBox();
			auto __widgetGroupEast = new QGroupBox();
			auto __buttonLayoutL = new QHBoxLayout();
			auto __buttonLayoutR = new QHBoxLayout();
			
			//Buttons
			auto __btnSpeakWest = m_widgets->add<TButton>(g_controlGroup,
					new TButton(QIcon(":Resources/icons/speak.png")),
					EWidgetID::ButtonSpeakWest);
			auto __btnTranslateWest = m_widgets->add<TButton>(g_controlGroup,
					new TButton(QIcon(":Resources/icons/translate.png")),
					EWidgetID::ButtonTranslateWest);
			auto __btnSpeakEast = m_widgets->add<TButton>(g_controlGroup,
					new TButton(QIcon(":Resources/icons/speak.png")),
					EWidgetID::ButtonSpeakEast);
			auto __btnTranslateEast = m_widgets->add<TButton>(g_controlGroup,
					new TButton(QIcon(":Resources/icons/translate.png")),
					EWidgetID::ButtonTranslateEast);
			
			__btnSpeakWest->setFixedSize(__buttonSize);
			__btnSpeakEast->setFixedSize(__buttonSize);
			__btnTranslateWest->setFixedSize(__buttonSize);
			__btnTranslateEast->setFixedSize(__buttonSize);
			
			// auto btnGroup = new QButtonGroup()
			__buttonLayoutL->addWidget(__btnSpeakWest);
			__buttonLayoutL->addWidget(__btnTranslateWest);
			__buttonLayoutR->addWidget(__btnSpeakEast);
			__buttonLayoutR->addWidget(__btnTranslateEast);
			
			// Set left box
			auto __optBoxL = new QHBoxLayout();
			__optBoxL->addWidget(new TLabel(tr("From"), __pLangBoxL));
			__optBoxL->addWidget(__pLangBoxL);
			
			__widgetLayoutWest->addLayout(__optBoxL);
			__widgetLayoutWest->addWidget(__pAreaWest);
			__widgetLayoutWest->addLayout(__buttonLayoutL);
			__widgetGroupWest->setLayout(__widgetLayoutWest);
			
			// Set central box
			auto __widgetLayoutCenter = new QVBoxLayout();
			auto __flipBtn = m_widgets->add<TButton>(
					g_controlGroup,
					new TButton(QIcon(":Resources/icons/flip.png")),
					EWidgetID::ButtonFlip
					);
			__flipBtn->setFixedSize(__buttonSize);
			__widgetLayoutCenter->addSpacing(44);
			__widgetLayoutCenter->addWidget(__pvoiceBox, 0, Qt::AlignmentFlag::AlignTop);
			__widgetLayoutCenter->addWidget(__flipBtn, 0, Qt::AlignmentFlag::AlignBottom);
			__widgetLayoutCenter->addSpacing(15);
			
			// Set right box
			auto __optBoxR = new QHBoxLayout();
			__optBoxR->addWidget(new TLabel(tr("To"), __pLangBoxR));
			__optBoxR->addWidget(__pLangBoxR);
			
			__widgetLayoutEast->addLayout(__optBoxR);
			__widgetLayoutEast->addWidget(__pAreaEast);
			__widgetLayoutEast->addLayout(__buttonLayoutR);
			__widgetGroupEast->setLayout(__widgetLayoutEast);
			
			// Main grid
			auto __pMainGrid = m_grids->add<QHBoxLayout>(
					g_layoutGroup, new QHBoxLayout(this),
					EWidgetID::Grid);
			__pMainGrid->addWidget(__widgetGroupWest);
			__pMainGrid->addLayout(__widgetLayoutCenter);
			__pMainGrid->addWidget(__widgetGroupEast);
			
			setValues();
			setLayout(__pMainGrid);
		}
		
		TStringPair
		TTranslatorPanel::getCurrentLanguage()
		{
			TString __langId {};
			TString __langName {};
			try
			{
				auto __pBox = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
				if (__pBox->isPlaceholderSet())
					return {};
				
				__langName = __pBox->currentText().toStdString();
				__langId = TConfigurationManager::getManager(EConfiguration::XML)
						->accessor()
						->getValue(NodeKey::kLangNameNode, __langName, NodeKey::kLangRoot).asUTF8();
			}
			catch (TException& e)
			{
				printerr(e.what())
			}
			return std::make_pair(__langId, __langName);
		}
		
		void TTranslatorPanel::onBufferChange()
		{ }
		
		void TTranslatorPanel::onFlip()
		{
			auto __areaWest = m_widgets->get<TTextEdit>(g_widgetGroup, EWidgetID::TranslatorAreaWest);
			auto __areaEast = m_widgets->get<TTextEdit>(g_widgetGroup, EWidgetID::TranslatorAreaEast);
			auto __optBoxWest = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
			auto __optBoxEast = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxEast);
			
			__optBoxWest->swapCurrent(__optBoxEast);
			__areaWest->swapContent(__areaEast);
		}
		
		void TTranslatorPanel::onSpeakWest()
		{
			auto __content = m_widgets->get<TTextEdit>(g_widgetGroup, EWidgetID::TranslatorAreaWest)
									  ->getContent();
			if (not __content.isEmpty())
			{
				#ifdef USE_SPEECHD
				if (not __langId.empty())
					m_narrator->setLanguage(TargetMode::DirAll, lang.first.c_str());
				m_narrator->say(SPD_TEXT, __content);
				#endif
			}
		}
		
		void TTranslatorPanel::onTranslateWest()
		{ }
		
		void TTranslatorPanel::onSpeakEast()
		{
			printl_log(__FUNCTION__)
		}
		
		void TTranslatorPanel::onTranslateEast()
		{
			printl_log(__FUNCTION__)
		}
		
		void TTranslatorPanel::onLanguageChangeFrom()
		{
			#ifdef USE_SPEECHD
			m_widgets->get<OptionBox>(g_widgetGroup, EWidgetID::LangboxWest)->onChanged(choiceEvent);
			TUString __langId {};
			auto __pLangBox = m_widgets->get<OptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
			auto __langName = __pLangBox->GetStringSelection();
			try
			{
				auto confMgr = ConfigurationManager::getManager();
				__langId = confMgr->xmlManager()->getValue(NodeKey::kLangRoot, NodeKey::kLangNameNode, __langName);
			} catch (...) {}
			m_narrator->setLanguage(TargetMode::DirAll, __langId);
			#endif
		}
		
		void TTranslatorPanel::onLanguageChangeTo()
		{ }
		
		void TTranslatorPanel::onTextAreaEdit()
		{ }
		
		void TTranslatorPanel::onVoiceChange()
		{
			#ifdef USE_SPEECHD
			QString __voiceId {};
			auto __voiceName = m_widgets->get<OptionBox>(g_widgetGroup, EWidgetID::VoiceBoxT)->GetStringSelection();

			try
			{
				auto confMgr = ConfigurationManager::getManager();
				__voiceId = confMgr->xmlManager()->getValue(NodeKey::kVoiceRoot, NodeKey::kVoiceNameNode, __voiceName);
				#ifdef DIXTER_DEBUG
			delete confMgr;
			#endif
			} catch (...) {}


			if (not __voiceId.empty())
				m_narrator->setSynthesisVoice(TargetMode::Single, __voiceId);
			printl_log(__voiceName)
			#endif
		}
		
		void TTranslatorPanel::connectEvents()
		{
			connect(
					m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonFlip),
					SIGNAL(clicked()), SLOT(onFlip()));
			connect(
					m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonSpeakWest),
					SIGNAL(clicked()), SLOT(onSpeakWest()));
			connect(
					m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonTranslateWest),
					SIGNAL(clicked()), SLOT(onTranslateWest()));
			
			connect(
					m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonSpeakEast),
					SIGNAL(clicked()), SLOT(onSpeakEast()));
			connect(
					m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonTranslateEast),
					SIGNAL(clicked()), SLOT(onTranslateEast()));
		}
		
		QWidget* TTranslatorPanel::getWidget(EWidgetID id)
		{
			return m_widgets->get(id);
		}
	} // namespace Gui
} //namespace Dixter