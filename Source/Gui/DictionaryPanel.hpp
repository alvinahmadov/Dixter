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
#include "OpenTranslate/LanguageBook.hpp"

namespace Dixter
{
	namespace Gui
	{
		class OptionBox;
		
		class DictionaryPanel : public Panel
		{
		Q_OBJECT
		private:
			using ColumnList    = std::list<ustring_t>;
			using WidgetGroup   = Group<QWidget, WidgetID>;
			using GridGroup     = Group<QLayout, WidgetID>;
		
		public:
			explicit DictionaryPanel(QWidget* parent, int width = -1, int height = -1,
			                         const QString& name = g_dictionaryName);
			
			virtual ~DictionaryPanel() dxDECL_OVERRIDE;
			
			void show(bool show = true);
			
			std::shared_ptr<OptionBox>
			getOptionBox(WidgetID widgetID);
			
			QWidget* getWidget(WidgetID id);
		
		protected:
			
			void init();
			
			void connectEvents() dxDECL_OVERRIDE;
			
			void setValues();
			
			void onCopyButton(void* event);
			
			void onClearButton(void* event);
			
			void onSearch(void*  event);
			
			void onLanguageChange(void*  event);
		
		private:
			bool m_isLanguageSet;
			
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
			
			OpenTranslate::LanguageBook* m_languages;
		};
	}
}
