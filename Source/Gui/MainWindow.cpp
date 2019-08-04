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

#include "Gui/MainWindow.hpp"
#include "Gui/Notebook.hpp"
#include "Gui/Panel.hpp"
#include "Gui/DictionaryPanel.hpp"
#include "Gui/TranslatorPanel.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/TextArea.hpp"
#include "Utilities.hpp"

using namespace Dixter::Utilities::Strings;

namespace Dixter
{
	namespace Gui
	{
		///DixterWindow Implementation
		DixterWindow::DixterWindow(const QString& title, int width, int height, bool visible)
				: WindowBase(title, width, height, visible, true, false),
				  m_widgets(new Group<QWidget, WidgetID>()),
				  m_dictionaryPanel {},
				  m_translatorPanel {}
		{
			init();
			showAll(visible);
			connectEvents();
		}
		
		DixterWindow::~DixterWindow() dxDECL_NOEXCEPT
		{
			SAFE_RELEASE(m_widgets)
		}
		
		void DixterWindow::showAll(bool show, bool showChildren)
		{
			setVisible(show);
			if (showChildren)
			{
				// m_widgets->forEach<void, bool>(g_widgetGroup, &QWidget::setVisible, showChildren);
			}
			if (m_dictionaryPanel != nullptr)
			{
				m_dictionaryPanel->show(showChildren);
			}
			if (m_translatorPanel != nullptr)
			{
				m_translatorPanel->show(showChildren);
			}
		}
		
		DictionaryPanel*
		DixterWindow::getDictionary()
		{
			return m_dictionaryPanel;
		};
		
		TranslatorPanel*
		DixterWindow::getTranslator()
		{
			return m_translatorPanel;
		};
		
		void DixterWindow::init()
		{
			auto ntbook = m_widgets->add<Notebook>(g_widgetGroup, new Notebook(this), WidgetID::Notebook);
			setCentralWidget(ntbook);
			initTranslator();
			initDictionary();
		}
		
		void DixterWindow::initDictionary()
		{
			auto __pNotebook = m_widgets->get<Notebook>(g_widgetGroup, WidgetID::Notebook);
			m_dictionaryPanel = new DictionaryPanel(__pNotebook, size().width(), size().height());
			
			/*
			m_dictionaryPanel->addAction(new ButtonAction("onCopyButton", [ ](Group<Widget, WidgetID>* widgets, wxCommandEvent& event)
			{
				auto __pSrchCtrl = widgets->get<SearchEntry>(g_controlGroup, WidgetID::ButtonLayout);
				__pSrchCtrl->SelectAll();
				__pSrchCtrl->Copy();
				event.Skip();
			}));
			
			m_dictionaryPanel->addAction(new ButtonAction("onClearButton", [ ](Group<Widget, WidgetID>* widgets, wxCommandEvent& event)
			{
				auto __pSrchCtrl = widgets->get<SearchEntry>(g_controlGroup, WidgetID::ButtonLayout);
				__pSrchCtrl->SelectAll();
				__pSrchCtrl->Cut();
				event.Skip();
			}));*/
			
			__pNotebook->addPage(m_dictionaryPanel, g_dictionaryName);
		}
		
		void DixterWindow::initTranslator()
		{
			auto __pNotebook = m_widgets->get<Notebook>(g_widgetGroup, WidgetID::Notebook);
			m_translatorPanel = new TranslatorPanel(__pNotebook, size().width(), size().height());
			
			/*
			m_translatorPanel->addAction(new SpeakerAction("onSpeakWest", [ ](Group<Widget, WidgetID>* widgets,
																					std::pair<QString, QString> lang,
																					wxFrame* frame,
																					wxCommandEvent& event)
			{
				auto __content = widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaWest)->getContent();
				if (!__content.empty())
				{
					#ifdef USE_SPEECHD
					if (!__langId.empty())
			m_narrator->setLanguage(TargetMode::kDirectionAll, lang.first.c_str());
		m_narrator->say(SPD_TEXT, __content);
					#endif
				}
				
				frame->PushStatusText("Talking...");
				event.Skip();
			}));
			
			
			m_translatorPanel->addAction(new ButtonAction("onFlip", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaWest)->swapContent(widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaEast));
				widgets->get<OptionBox>(g_controlGroup, WidgetID::LangboxWest)->interchange(widgets->get<OptionBox>(g_controlGroup, WidgetID::LangboxEast));
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onCopyButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaWest)->copyToClipboard(true);
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onCutButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaWest)->cutToClipboard();
				event.Skip();
			}));
			
			m_translatorPanel->addAction(new ButtonAction("onClearButtonWest", [ ](WidgetGroup* widgets, wxCommandEvent& event)
			{
				widgets->get<TextArea>(g_widgetGroup, WidgetID::AreaWest)->clearContent();
				event.Skip();
			}));
			*/
			
			__pNotebook->addPage(m_translatorPanel, g_translatorName);
		}
		
		void DixterWindow::connectEvents()
		{
		
		}
	}
}
