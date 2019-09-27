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
	namespace NAlgoUtils = Utilities::Algorithms;
	
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
			auto __languageNames = std::vector<TUString>();
			auto __languageDisplayNames = std::vector<TUString>();
			auto __languageIds = std::vector<TUString>();
			auto __voiceNames = std::vector<TUString>();
			
			try
			{
				TConfigurationManager::getManager(EConfiguration::XML)
						->accessor()
						->getValues(NodeKey::kLangNameNode, __languageNames, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangNameDisplayNode, __languageDisplayNames, NodeKey::kLangRoot)
						->getValues(NodeKey::kLangIdNode, __languageIds, NodeKey::kLangRoot)
						->getValues(NodeKey::kVoiceNameNode, __voiceNames, NodeKey::kVoiceRoot);
			} catch (std::exception& e)
			{ printerr(e.what()); }
			
			NAlgoUtils::foreachCompound(__languageNames, __languageDisplayNames,
										[](TUString& languageName, const TUString& languageDisplayName)
										{ languageName.append(u" / ").append(languageDisplayName); });
			
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest)->setValues(__languageNames);
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxEast)->setValues(__languageNames);
			m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::VoiceBoxT)->setValues(__voiceNames);
		}
		
		void TTranslatorPanel::init()
		{
			// init widgets
			const QSize __buttonSize = QSize(150, 50);
			auto __voiceBox = m_widgets
					->add<TOptionBox>(g_widgetGroup,
									  new TOptionBox(nullptr, tr("Select voice...")),
									  EWidgetID::VoiceBoxT);
			auto __texteditWest = m_widgets
					->add<TTextEdit>(g_widgetGroup,
									 new TTextEdit(this, tr("Translation")),
									 EWidgetID::TranslatorAreaWest);
			auto __texteditEast = m_widgets
					->add<TTextEdit>(g_widgetGroup,
									 new TTextEdit(this, tr("Translation"), true),
									 EWidgetID::TranslatorAreaEast);
			auto __languageBoxWest = m_widgets
					->add<TOptionBox>(g_widgetGroup,
									  new TOptionBox(tr("Select language...")),
									  EWidgetID::LangboxWest);
			auto __languageBoxEast = m_widgets
					->add<TOptionBox>(g_widgetGroup,
									  new TOptionBox(tr("Select language...")),
									  EWidgetID::LangboxEast);
			
			auto __widgetLayoutWest = new QVBoxLayout();
			auto __widgetLayoutEast = new QVBoxLayout();
			auto __widgetGroupWest = new QGroupBox();
			auto __widgetGroupEast = new QGroupBox();
			auto __buttonLayoutWest = new QHBoxLayout();
			auto __buttonLayoutEast = new QHBoxLayout();
			
			//Buttons
			auto __speakButtonWest = m_widgets
					->add<TButton>(g_controlGroup,
								   new TButton(QIcon(":Resources/icons/speak.png")),
								   EWidgetID::ButtonSpeakWest);
			auto __translateButtonWest = m_widgets
					->add<TButton>(g_controlGroup,
								   new TButton(QIcon(":Resources/icons/translate.png")),
								   EWidgetID::ButtonTranslateWest);
			auto __speakButtonEast = m_widgets
					->add<TButton>(g_controlGroup,
								   new TButton(QIcon(":Resources/icons/speak.png")),
								   EWidgetID::ButtonSpeakEast);
			auto __translateButtonEast = m_widgets
					->add<TButton>(g_controlGroup,
								   new TButton(QIcon(":Resources/icons/translate.png")),
								   EWidgetID::ButtonTranslateEast);
			
			__speakButtonWest->setFixedSize(__buttonSize);
			__speakButtonEast->setFixedSize(__buttonSize);
			__translateButtonWest->setFixedSize(__buttonSize);
			__translateButtonEast->setFixedSize(__buttonSize);
			
			// auto btnGroup = new QButtonGroup()
			__buttonLayoutWest->addWidget(__speakButtonWest);
			__buttonLayoutWest->addWidget(__translateButtonWest);
			__buttonLayoutEast->addWidget(__speakButtonEast);
			__buttonLayoutEast->addWidget(__translateButtonEast);
			
			// Set left box
			auto __optionBoxWest = new QHBoxLayout();
			__optionBoxWest->addWidget(new TLabel(tr("From"), __languageBoxWest));
			__optionBoxWest->addWidget(__languageBoxWest);
			
			__widgetLayoutWest->addLayout(__optionBoxWest);
			__widgetLayoutWest->addWidget(__texteditWest);
			__widgetLayoutWest->addLayout(__buttonLayoutWest);
			__widgetGroupWest->setLayout(__widgetLayoutWest);
			
			// Set central box
			auto __widgetLayoutCenter = new QVBoxLayout;
			auto __flipButton = m_widgets
					->add<TButton>(g_controlGroup,
								   new TButton(QIcon(":Resources/icons/flip.png")),
								   EWidgetID::ButtonFlip);
			
			__flipButton->setFixedSize(__buttonSize);
			__widgetLayoutCenter->addSpacing(44);
			__widgetLayoutCenter->addWidget(__voiceBox, 0, Qt::AlignmentFlag::AlignTop);
			__widgetLayoutCenter->addWidget(__flipButton, 0, Qt::AlignmentFlag::AlignBottom);
			__widgetLayoutCenter->addSpacing(15);
			
			// Set right box
			auto __optionBoxEast = new QHBoxLayout();
			__optionBoxEast->addWidget(new TLabel(tr("To"), __languageBoxEast));
			__optionBoxEast->addWidget(__languageBoxEast);
			
			__widgetLayoutEast->addLayout(__optionBoxEast);
			__widgetLayoutEast->addWidget(__texteditEast);
			__widgetLayoutEast->addLayout(__buttonLayoutEast);
			__widgetGroupEast->setLayout(__widgetLayoutEast);
			
			// Main grid
			auto __mainGrid = m_grids->add<QHBoxLayout>(g_layoutGroup, new QHBoxLayout(this),
														EWidgetID::Grid);
			__mainGrid->addWidget(__widgetGroupWest);
			__mainGrid->addLayout(__widgetLayoutCenter);
			__mainGrid->addWidget(__widgetGroupEast);
			
			setValues();
			setLayout(__mainGrid);
		}
		
		TStringPair
		TTranslatorPanel::getCurrentLanguage()
		{
			TString __languageId, __languageName;
			
			try
			{
				auto __languageBoxWest = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
				if (__languageBoxWest->isPlaceholderSet())
					return {};
				
				__languageName = __languageBoxWest->currentText().toStdString();
				__languageId = getXmlManager({ g_langConfigPath })
						->accessor()
						->getValue(NodeKey::kLangNameNode, __languageName, NodeKey::kLangRoot).asUTF8();
			}
			catch (TException& e)
			{
				printerr(e.what())
			}
			return std::make_pair(__languageId, __languageName);
		}
		
		void TTranslatorPanel::onBufferChange()
		{ }
		
		void TTranslatorPanel::onFlip()
		{
			auto __texteditWest = m_widgets->get<TTextEdit>(g_widgetGroup, EWidgetID::TranslatorAreaWest);
			auto __languageBoxWest = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxWest);
			auto __texteditEast = m_widgets->get<TTextEdit>(g_widgetGroup, EWidgetID::TranslatorAreaEast);
			auto __languageBoxEast = m_widgets->get<TOptionBox>(g_widgetGroup, EWidgetID::LangboxEast);
			
			__languageBoxWest->swapCurrent(__languageBoxEast);
			__texteditWest->swapContent(__texteditEast);
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
			connect(m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonFlip),
					SIGNAL(clicked()), SLOT(onFlip()));
			connect(m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonSpeakWest),
					SIGNAL(clicked()), SLOT(onSpeakWest()));
			connect(m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonTranslateWest),
					SIGNAL(clicked()), SLOT(onTranslateWest()));
			connect(m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonSpeakEast),
					SIGNAL(clicked()), SLOT(onSpeakEast()));
			connect(m_widgets->get<TButton>(g_controlGroup, EWidgetID::ButtonTranslateEast),
					SIGNAL(clicked()), SLOT(onTranslateEast()));
		}
		
		QWidget* TTranslatorPanel::getWidget(EWidgetID id)
		{
			return m_widgets->get(id);
		}
	} // namespace Gui
} //namespace Dixter