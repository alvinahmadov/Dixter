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

#include "Group.hpp"
#include "Constants.hpp"
#include "Configuration.hpp"
#include "Gui/Window.hpp"
#include "Gui/Notebook.hpp"
#include "Gui/DictionaryPanel.hpp"
#include "Gui/TranslatorPanel.hpp"
#include "Gui/TextEdit.hpp"

namespace Dixter
{
	namespace Gui
	{
		// TWindow Implementation
		TWindow::TWindow(const QString& title, int width, int height, bool visible)
				: TBase(title, width, height, visible, false, false),
				  m_widgets(new TGroup<QWidget, EWidgetID>()),
				  m_dictionaryPanel(),
				  m_translatorPanel()
		{
			init();
			showAll(visible, visible);
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
				m_widgets->forEach(g_widgetGroup, &QWidget::setVisible, showChildren);
			
			if (m_dictionaryPanel)
				m_dictionaryPanel->show(showChildren);
			
			if (m_translatorPanel)
				m_translatorPanel->show(showChildren);
		}
		
		TWindow::TDictionaryPanelPtr
		TWindow::getDictionary()
		{
			return dxMAKE_SHARED(TDictionaryPanel, m_dictionaryPanel);
		};
		
		TWindow::TTranslatorPanelPtr
		TWindow::getTranslator()
		{
			return dxMAKE_SHARED(TTranslatorPanel, m_translatorPanel);
		};
		
		void TWindow::init()
		{
			auto __confMan = getIniManager({ g_guiConfigPath })->accessor();
			auto __backgroundColour = __confMan->getValue(NodeKey::kWinBgColourNode).asCustom();
			auto __fontName = __confMan->getValue(NodeKey::kWinFontNameNode).asCustom();
			int  __fontSize = __confMan->getValue(NodeKey::kWinFontSizeNode);
			
			__backgroundColour.prepend('#');
			setPalette(QPalette(__backgroundColour));
			setFont(QFont(__fontName, __fontSize));
			
			setCentralWidget(m_widgets->add<TNotebook>(g_widgetGroup, new TNotebook(this),
													   EWidgetID::Notebook));
			initDictionary();
			initTranslator();
		}
		
		void TWindow::connectEvents()
		{ }
		
		void TWindow::initDictionary()
		{
			auto __pNotebook = m_widgets->get<TNotebook>(g_widgetGroup, EWidgetID::Notebook);
			m_dictionaryPanel = new TDictionaryPanel(__pNotebook, size().width(), size().height());
			__pNotebook->addPage(m_dictionaryPanel, g_dictionaryName);
		}
		
		void TWindow::initTranslator()
		{
			auto __pNotebook = m_widgets->get<TNotebook>(g_widgetGroup, EWidgetID::Notebook);
			m_translatorPanel = new TTranslatorPanel(__pNotebook, size().width(), size().height());
			__pNotebook->addPage(m_translatorPanel, g_translatorName);
		}
	}
}