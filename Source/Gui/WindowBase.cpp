/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QCoreApplication>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include "Group.hpp"
#include "Constants.hpp"
#include "Gui/WindowBase.hpp"
#include "Gui/SettingsDialog.hpp"

namespace Dixter
{
	namespace Gui
	{
		TWindowBase::TWindowBase()
				: QMainWindow(),
				  m_hasMenus(false),
				  m_hasStatusBar(false)
		{
			init();
			connectEvents();
		}
		
		TWindowBase::TWindowBase(const QString& title, int width, int height,
								 bool visible, bool enableMenu, bool enableStatusbar)
				: TWindowBase(nullptr, title, width, height, visible, enableMenu, enableStatusbar)
		{
			init();
			setWindowTitle(title);
			setMinimumSize(width, height);
			setVisible(visible);
		}
		
		TWindowBase::TWindowBase(QWidget* parent, const QString& title, int width, int height,
								 bool visible, bool enableMenu, bool enableStatusbar)
				: QMainWindow(parent),
				  m_hasMenus { enableMenu },
				  m_hasStatusBar { enableStatusbar },
				  m_objects { new TGroup<QObject, EMenuID> },
				  m_statbar { nullptr }
		{
			init();
			setWindowTitle(title);
			setMinimumSize(width, height);
			setVisible(visible);
		}
		
		TWindowBase::~TWindowBase()
		{
			delete m_objects;
		}
		
		QWidget*
		TWindowBase::getChildWidget(const QString& name)
		{
			return findChild<QWidget*>(name);
		}
		
		void TWindowBase::init()
		{
			m_settingsDialog = new TSettingsDialog(this, QString("Settings"));
			if (m_hasMenus)
			{
				auto __menuBar = new QMenuBar(this);
				auto __menuFile = new QMenu(tr("File"), __menuBar);
				auto __menuEdit = new QMenu(tr("Edit"), __menuBar);
				auto __menuView = new QMenu(tr("View"), __menuBar);
				//File menu
				__menuFile->addMenu(tr("&New"));
				__menuFile->addMenu(tr("&Open"));
				
				auto __quitAction = new QAction(tr("&Quit"));
				__menuFile->addAction(__quitAction);
				
				connect(__quitAction, SIGNAL(triggered()), this, SLOT(close()));
				
				//Edit menu
				__menuEdit->addMenu(tr("&Preferences"));
				//View menu
				
				__menuBar->addMenu(__menuFile);
				__menuBar->addMenu(__menuEdit);
				__menuBar->addMenu(__menuView);
				
				__menuBar->show();
				setMenuBar(__menuBar);
			}
			
			if (m_hasStatusBar)
			{
				
				m_statbar = new QStatusBar(this);
				setStatusBar(m_statbar);
				m_statbar->show();
			}
		}
		
		void TWindowBase::onMenuitemNew(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemOpen(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemQuit(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemCopy(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemPaste(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemPrefs(QMenuEvent*)
		{
			m_settingsDialog->show();
		}
		
		void TWindowBase::onMenuitemSortName(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemSortDate(QMenuEvent*)
		{ }
		
		void TWindowBase::onMenuitemSortType(QMenuEvent*)
		{ }
		
		void TWindowBase::onQuit()
		{
			QCoreApplication::quit();
		}
		
		void TWindowBase::setSize(Int32 width, Int32 height)
		{
			setMinimumSize(width, height);
		}
		
		void TWindowBase::connectEvents()
		{
			//	BIND_EVENT(wxEVT_UPDATE_UI, Window::OnTextEdit)
			// BIND_EVENT_B(wxEVT_MENU, DixterWindow, onMenuitemNew, EMenuID, IdFileNew);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemOpen, EMenuID, IdFileOpen);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemQuit, EMenuID, IdFileQuit);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemCopy, EMenuID, IdEditCopy);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemPaste, EMenuID, IdEditPaste);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemPrefs, EMenuID, IdEditPreferences);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortName, EMenuID, IdViewSortName);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortDate, EMenuID, IdViewSortDate);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortType, EMenuID, IdViewSortType);
		}
	}
}