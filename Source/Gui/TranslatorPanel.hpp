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

#include <boost/functional.hpp>
#include <QLayoutItem>

#include "Group.hpp"
#include "Gui/Panel.hpp"

#ifdef USE_SPEECHD
#include "SpeechSynthesizer/SpeechDispatcher.h"
#endif


namespace Dixter
{
	namespace Gui
	{
		class TOptionBox;
		class TTextEdit;
		class TTranslatorPanel : public TPanel
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
			                 const QString& name = g_translatorName);
			
			~TTranslatorPanel() override;
			
			void show(bool show);
			
			std::shared_ptr<TOptionBox>
			getOptionBox(EWidgetID id);
			
			QWidget* getWidget(EWidgetID id);
			
			std::pair<TString, TString>
			getCurrentLanguage();
		
		protected:
			
			void init();
			
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
			#ifdef USE_SPEECHD
			vsynth::SpeechDispatcher *m_narrator;
			#endif
			
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
		};
	}
}