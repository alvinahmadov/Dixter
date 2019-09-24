/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include "Group.hpp"
#include "Commons.hpp"
#include "Gui/Panel.hpp"
#include "Gui/SearchEntry.hpp"

namespace Dixter
{
	namespace Database
	{
		class TManager;
	}
	
	namespace Gui
	{
		class TOptionBox;
		
		class TDictionaryPanel : public TPanel
		{
		Q_OBJECT
		private:
			using ColumnList    = std::list<TUString>;
			using WidgetGroup   = TGroup<QWidget, EWidgetID>;
			using GridGroup     = TGroup<QLayout, EWidgetID>;
		
		public:
			explicit TDictionaryPanel(QWidget* parent, int width = -1, int height = -1,
			                          const QString& name = g_dictionaryName);
			
			virtual ~TDictionaryPanel() override;
			
			void show(bool show = true);
			
			std::shared_ptr<TOptionBox>
			getOptionBox(EWidgetID widgetID);
			
			QWidget* getWidget(EWidgetID id);
		
		protected:
			
			void init();
			
			void connectEvents() override;
			
			void setValues();
			
			void onCopyButton(void);
			
			void onClearButton(void);
			
			void onLanguageChange(void);
		protected slots:
			
			void onSearch(const QString& text);
		
		private:
			bool m_isLanguageSet;
			
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
		};
	}
}
