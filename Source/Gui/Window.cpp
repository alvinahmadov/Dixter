/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <boost/filesystem.hpp>

#include <QDialog>
#include <QMenu>
#include <QEvent>
#include <QString>
#include <QMenuBar>

#include "Gui/Window.hpp"
#include "Gui/Notebook.hpp"
#include "Gui/Panel.hpp"
#include "Gui/DictionaryPanel.hpp"
#include "Gui/TranslatorPanel.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/TextEdit.hpp"
#include "Utilities.hpp"

using namespace Dixter::Utilities::Strings;

namespace Dixter
{
	namespace Gui
	{
		///DixterWindow Implementation
		TWindow::TWindow(const QString& title, int width, int height, bool visible)
				: TWindowBase(title, width, height, visible, true, false),
				  m_widgets(new TGroup<QWidget, EWidgetID>()),
				  m_dictionaryPanel(),
				  m_translatorPanel()
		{
			init();
			showAll(visible);
			
			connectEvents();
		}
		
		TWindow::~TWindow() noexcept
		{
			delete m_widgets;
		}
		
		void TWindow::showAll(bool show, bool showChildren)
		{
			setVisible(show);
			if (showChildren)
				m_widgets->forEach<void, bool>(g_widgetGroup, &QWidget::setVisible, showChildren);
			
			if (m_dictionaryPanel)
				m_dictionaryPanel->show(showChildren);
			
			if (m_translatorPanel)
				m_translatorPanel->show(showChildren);
		}
		
		TDictionaryPanel*
		TWindow::getDictionary()
		{
			return m_dictionaryPanel;
		};
		
		TTranslatorPanel*
		TWindow::getTranslator()
		{
			return m_translatorPanel;
		};
		
		void TWindow::init()
		{
			auto __ntbook = m_widgets->add<TNotebook>(g_widgetGroup, new TNotebook(this), EWidgetID::Notebook);
			setCentralWidget(__ntbook);
			initTranslator();
			initDictionary();
		}
		
		void TWindow::initDictionary()
		{
			auto __pNotebook = m_widgets->get<TNotebook>(g_widgetGroup, EWidgetID::Notebook);
			m_dictionaryPanel = new TDictionaryPanel(__pNotebook, size().width(), size().height());
			
			/*
			m_dictionaryPanel->addAction(new ButtonAction("onCopyButton", [ ](TGroup<Widget, EWidgetID>* widgets, wxCommandEvent& event)
			{
				auto __pSrchCtrl = widgets->get<SearchEntry>(g_controlGroup, EWidgetID::ButtonLayout);
				__pSrchCtrl->SelectAll();
				__pSrchCtrl->Copy();
				event.Skip();
			}));
			
			m_dictionaryPanel->addAction(new ButtonAction("onClearButton", [ ](TGroup<Widget, EWidgetID>* widgets, wxCommandEvent& event)
			{
				auto __pSrchCtrl = widgets->get<SearchEntry>(g_controlGroup, EWidgetID::ButtonLayout);
				__pSrchCtrl->SelectAll();
				__pSrchCtrl->Cut();
				event.Skip();
			}));*/
			
			__pNotebook->addPage(m_dictionaryPanel, g_dictionaryName);
		}
		
		void TWindow::initTranslator()
		{
			auto __pNotebook = m_widgets->get<TNotebook>(g_widgetGroup, EWidgetID::Notebook);
			m_translatorPanel = new TTranslatorPanel(__pNotebook, size().width(), size().height());
			
			/*
			m_translatorPanel->addAction(new SpeakerAction("onSpeakWest", [ ](TGroup<Widget, EWidgetID>* widgets,
																					std::pair<QString, QString> lang,
																					wxFrame* frame,
																					wxCommandEvent& event)
			{
				auto __content = widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaWest)->getContent();
				if (!__content.empty())
				{
					#ifdef USE_SPEECHD
					if (!__langId.empty())
			m_narrator->setLanguage(TargetMode::DirAll, lang.first.c_str());
		m_narrator->say(SPD_TEXT, __content);
					#endif
				}
				
				frame->PushStatusText("Talking...");
				event.Skip();
			}));
			
			
			m_translatorPanel->addAction(new ButtonAction("onFlip", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaWest)->swapContent(widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaEast));
				widgets->get<OptionBox>(g_controlGroup, EWidgetID::LangboxWest)->interchange(widgets->get<OptionBox>(g_controlGroup, EWidgetID::LangboxEast));
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onCopyButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaWest)->copyToClipboard(true);
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onCutButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaWest)->cutToClipboard();
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onClearButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, EWidgetID::AreaWest)->clearContent();
				event.Skip();
			}));
			*/
			
			__pNotebook->addPage(m_translatorPanel, g_translatorName);
		}
		
		void TWindow::connectEvents()
		{ }
	}
}
