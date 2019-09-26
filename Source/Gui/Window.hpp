/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <memory>
#include "WindowBase.hpp"

namespace Dixter
{
	namespace Gui
	{
		class APanel;
		
		class TTranslatorPanel;
		
		class TDictionaryPanel;
		
		class TSettingsDialog;
		
		/**
		 * Main window
		 * */
		class TWindow : public TWindowBase
		{
		Q_OBJECT
		private:
			using TBase = TWindowBase;
		
		public:
			using TDictionaryPanelPtr = std::shared_ptr<TDictionaryPanel>;
			using TTranslatorPanelPtr = std::shared_ptr<TTranslatorPanel>;
		
		public:
			explicit TWindow(const QString& title, int width = -1, int height = -1, bool visible = true);
			
			virtual ~TWindow() noexcept override;
			
			void showAll(bool visible = true, bool showChildren = true);
			
			TDictionaryPanelPtr
			getDictionary();
			
			TTranslatorPanelPtr
			getTranslator();
		
		protected:
			
			void init() override;
			
			void connectEvents() override;
			
			void initDictionary();
			
			void initTranslator();
		
		private:
			TGroup<QWidget, EWidgetID>* m_widgets;
			
			TDictionaryPanel* m_dictionaryPanel;
			
			TTranslatorPanel* m_translatorPanel;
		};
	}
}