/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include "Gui/Panel.hpp"
#include "Gui/OptionBox.hpp"

class QMutex;

namespace Dixter
{
	enum class EWidgetID;
	
	template<
			typename T,
			typename ID
	>
	class TGroup;
	
	namespace Gui
	{
		class TDictionaryPanel : public TPanel
		{
		Q_OBJECT
		private:
			using WidgetGroup   = TGroup<QWidget, EWidgetID>;
			using GridGroup     = TGroup<QLayout, EWidgetID>;
		
		public:
			explicit TDictionaryPanel(QWidget* parent, int width = -1, int height = -1,
									  const QString& name = QString("Dictionary"));
			
			virtual ~TDictionaryPanel() override;
			
			void show(bool show = true);
			
			TOptionBoxPtr
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
			
			void onSearch() noexcept;
		
		private:
			bool m_isLanguageSet;
			
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
		};
	} // namespace Gui
} // namespace Dixter