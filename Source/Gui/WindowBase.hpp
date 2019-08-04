/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Constants.hpp"
#include "Group.hpp"

#include <QMainWindow>

class QObject;
class QWidget;
class QSize;
class QStatusBar;
class QMenuEvent;

namespace Dixter
{
	namespace Gui
	{
		class SettingsDialog;
		class WindowBase : public QMainWindow
		{
		Q_OBJECT
		public:
			using MenuEvent    = QMenuEvent*;
			using ObjectGroup  = Group<QObject, MenuID>;
			using WidgetGroup  = Group<QWidget, WidgetID>;
			using ControlGroup = Group<QObject, WidgetID>;
		
		public:
			WindowBase();
			
			explicit WindowBase(const QString& title, int width, int height,
			                    bool visible = true, bool hasMenu = true, bool hasStatusbar = true);
			
			explicit WindowBase(QWidget* parent, const QString& title, int width, int height,
			                    bool visible = true, bool enableMenu = true, bool statusbar = false);
			
			virtual ~WindowBase() dxDECL_OVERRIDE;
			
			QWidget* getChildWidget(const QString& childName);
		
		protected:
			virtual void init();
			
			void onMenuitemNew(QMenuEvent* menuEvent);
			
			void onMenuitemOpen(QMenuEvent* menuEvent);
			
			void onMenuitemQuit(QMenuEvent* menuEvent);
			
			void onMenuitemCopy(QMenuEvent* menuEvent);
			
			void onMenuitemPaste(QMenuEvent* menuEvent);
			
			void onMenuitemPrefs(QMenuEvent* menuEvent);
			
			void onMenuitemSortName(QMenuEvent* menuEvent);
			
			void onMenuitemSortDate(QMenuEvent* menuEvent);
			
			void onMenuitemSortType(QMenuEvent* menuEvent);
			
			void onQuit();
			
			virtual void setSize(i32 width, i32 height);
			
			virtual void connectEvents();
		
		private:
			bool m_hasMenus;
			
			bool m_hasStatusBar;
			
			Group<QObject, MenuID>* m_objects;
			
			QStatusBar* m_statbar;
			
			SettingsDialog* m_settingsDialog;
		};
	}
}


