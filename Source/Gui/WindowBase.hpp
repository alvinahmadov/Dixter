/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QMainWindow>

class QObject;
class QWidget;
class QSize;
class QStatusBar;
class QMenuEvent;

namespace Dixter
{
	template<
			typename T,
			typename ID
	>
	class TGroup;
	
	enum class EMenuID;
	enum class EWidgetID;
	
	namespace Gui
	{
		class TSettingsDialog;
		
		class TWindowBase : public virtual QMainWindow
		{
		Q_OBJECT
		public:
			using ObjectGroup  = TGroup<QObject, EMenuID>;
			using WidgetGroup  = TGroup<QWidget, EWidgetID>;
			using ControlGroup = TGroup<QObject, EWidgetID>;
		
		public:
			TWindowBase();
			
			explicit TWindowBase(const QString& title, int width, int height,
						bool visible = true, bool hasMenu = true, bool hasStatusbar = true);
			
			TWindowBase(QWidget* parent, const QString& title, int width, int height,
						bool visible = true, bool enableMenu = true, bool statusbar = false);
			
			virtual ~TWindowBase() override;
			
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
			
			virtual void setSize(int width, int height);
			
			virtual void connectEvents();
		
		private:
			bool m_hasMenus;
			
			bool m_hasStatusBar;
			
			TGroup<QObject, EMenuID>* m_objects;
			
			QStatusBar* m_statbar;
			
			TSettingsDialog* m_settingsDialog;
		};
	}
}


