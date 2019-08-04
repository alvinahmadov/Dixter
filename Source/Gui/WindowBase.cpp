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
#include <QDialog>

#include "Group.hpp"
#include "Gui/WindowBase.hpp"
#include "Gui/SettingsDialog.hpp"

namespace Dixter
{
	namespace Gui
	{
		WindowBase::WindowBase()
				: QMainWindow(),
				  m_hasMenus { false },
				  m_hasStatusBar { false }
		{
			init();
			connectEvents();
		}
		
		WindowBase::WindowBase(const QString& title, int width, int height,
		                       bool visible, bool enableMenu, bool enableStatusbar)
				: WindowBase(nullptr, title, width, height, visible, enableMenu, enableStatusbar)
		{
			init();
			setWindowTitle(title);
			setMinimumSize(width, height);
			setVisible(visible);
		}
		
		WindowBase::WindowBase(QWidget* parent, const QString& title, int width, int height,
		                       bool visible, bool enableMenu, bool enableStatusbar)
				: QMainWindow(parent),
				  m_hasMenus { enableMenu },
				  m_hasStatusBar { enableStatusbar },
				  m_objects { new Group<QObject, MenuID> },
				  m_statbar { nullptr }
		{
			init();
			setWindowTitle(title);
			setMinimumSize(width, height);
			setVisible(visible);
		}
		
		WindowBase::~WindowBase()
		{
			SAFE_RELEASE(m_objects)
		}
		
		QWidget*
		WindowBase::getChildWidget(const QString& name)
		{
			return findChild<QWidget*>(name);
		}
		
		void WindowBase::init()
		{
			m_settingsDialog = new SettingsDialog(this, QString("Settings"));
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
		
		void WindowBase::onMenuitemNew(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemOpen(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemQuit(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemCopy(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemPaste(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemPrefs(QMenuEvent*)
		{
			m_settingsDialog->show();
		}
		
		void WindowBase::onMenuitemSortName(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemSortDate(QMenuEvent*)
		{ }
		
		void WindowBase::onMenuitemSortType(QMenuEvent*)
		{ }
		
		void WindowBase::onQuit()
		{
			QCoreApplication::quit();
		}
		
		void WindowBase::setSize(i32 width, i32 height)
		{
			setMinimumSize(width, height);
		}
		
		void WindowBase::connectEvents()
		{
			//	BIND_EVENT(wxEVT_UPDATE_UI, Window::OnTextEdit)
			// BIND_EVENT_B(wxEVT_MENU, DixterWindow, onMenuitemNew, MenuID, IdFileNew);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemOpen, MenuID, IdFileOpen);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemQuit, MenuID, IdFileQuit);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemCopy, MenuID, IdEditCopy);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemPaste, MenuID, IdEditPaste);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemPrefs, MenuID, IdEditPreferences);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortName, MenuID, IdViewSortName);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortDate, MenuID, IdViewSortDate);
			// BIND_EVENT_B(wxEVT_MENU, Window, onMenuitemSortType, MenuID, IdViewSortType);
		}
	}
}