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
	namespace OpenTranslate
	{
		class LanguageBook;
	}
	namespace Gui
	{
		class OptionBox;
		class TextArea;
		class TranslatorPanel : public Panel
		{
		Q_OBJECT
			
			#ifdef USE_SPEECHD
			using SD = vsynth::SpeechDispatcher;
			#endif
			using WidgetGroup   = Group<QWidget, WidgetID>;
			using GridGroup     = Group<QLayoutItem, WidgetID>;
		
		public:
			explicit TranslatorPanel(QWidget* parent,
			                         int width = 200, int height = 200,
			                         const QString& name = g_translatorName);
			
			virtual ~TranslatorPanel() dxDECL_OVERRIDE;
			
			void show(bool show);
			
			std::shared_ptr<OptionBox>
			getOptionBox(WidgetID id);
			
			QWidget* getWidget(WidgetID id);
			
			std::pair<string_t, string_t>
			getCurrentLanguage();
		
		protected:
			
			void init();
			
			void connectEvents() dxDECL_OVERRIDE;
			
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
			
			OpenTranslate::LanguageBook* m_lgs;
			
			GridGroup* m_grids;
			
			WidgetGroup* m_widgets;
		};
	}
}