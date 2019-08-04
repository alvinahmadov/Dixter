/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "WindowBase.hpp"

namespace Dixter
{
	namespace Gui
	{
		class Panel;
		
		class TranslatorPanel;
		
		class DictionaryPanel;
		
		class SettingsDialog;
		
		/**
		 * Main window
		 * */
		class DixterWindow : public WindowBase
		{
		Q_OBJECT
		public:
			explicit DixterWindow(const QString& title, int width = -1, int height = -1, bool visible = true);
			
			virtual ~DixterWindow() dxDECL_NOEXCEPT dxDECL_OVERRIDE;
			
			void showAll(bool visible = true, bool showChildren = true);
			
			DictionaryPanel* getDictionary();
			
			TranslatorPanel* getTranslator();
		
		protected:
			
			void init() dxDECL_OVERRIDE;
			
			void connectEvents() dxDECL_OVERRIDE;
			
			void initDictionary();
			
			void initTranslator();
		
		private:
			Group <QWidget, WidgetID>* m_widgets;
			
			DictionaryPanel* m_dictionaryPanel = nullptr;
			
			TranslatorPanel* m_translatorPanel = nullptr;
		};
		
	}
}