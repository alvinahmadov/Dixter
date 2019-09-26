/**
 *  File TranslatorPanel.h
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Gui/Panel.hpp"

#ifdef USE_SPEECHD
#include "SpeechSynthesizer/SpeechDispatcher.h"
#endif

class QLayoutItem;

namespace Dixter
{
	template<
			typename T,
			typename ID
	>
	class TGroup;
	
	namespace Gui
	{
		class TTextEdit;
		
		class TTranslatorPanel : public APanel
		{
		Q_OBJECT
			
			#ifdef USE_SPEECHD
			using SD = vsynth::SpeechDispatcher;
			#endif
			using WidgetGroup   = TGroup<QWidget, EWidgetID>;
			using GridGroup     = TGroup<QLayoutItem, EWidgetID>;
		
		public:
			TTranslatorPanel(QWidget* parent,
			                 int width = 200, int height = 200,
			                 const QString& name = QString());
			
			~TTranslatorPanel() override;
			
			void show(bool show = true);
			
			TOptionBoxPtr
			getOptionBox(EWidgetID id);
			
			QWidget*
			getWidget(EWidgetID id);
			
			TStringPair
			getCurrentLanguage();
		
		protected:
			void init() override;
			
			void connectEvents() override;
			
			void setValues();
		
		protected slots:
			void onBufferChange();
			
			void onFlip();
			
			void onSpeakWest();
			
			void onTranslateWest();
			
			void onSpeakEast();
			
			void onTranslateEast();
			
			void onLanguageChangeFrom();
			
			void onLanguageChangeTo();
			
			void onTextAreaEdit();
			
			void onVoiceChange();
			
		private:
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
			
			#ifdef USE_SPEECHD
			vsynth::SpeechDispatcher *m_narrator;
			#endif
		};
	}
}