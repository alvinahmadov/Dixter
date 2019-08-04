/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <QString>

namespace Dixter
{
	namespace
	{
		static const QString g_translatorName {"Translator"};
		static const QString g_dictionaryName {"Dictionary"};
		static const string_t g_widgetGroup {"widget"};
		static const string_t g_controlGroup {"control"};
		static const string_t g_menuGroup {"menu"};
		static const string_t g_layoutGroup {"sizer"};
		
		const string_t g_searhPlaceholder {"Search word..."};
		
		struct NodeKey
		{
			static inline const string_t kLangRoot              {"languages"};
			static inline const string_t kVoiceRoot             {"voices"};
			static inline const string_t kSettingsRoot          {"settings"};
			
			static inline const string_t kVoiceNode             {"voices.voice"};
			static inline const string_t kVoiceNameNode         {"voices.voice.name"};
			static inline const string_t kVoiceValueNode        {"voices.voice.value"};
			static inline const string_t kLangNode              {"languages.language"};
			static inline const string_t kLangIdNode            {"languages.language.id"};
			static inline const string_t kLangNameNode          {"languages.language.name"};
			static inline const string_t kLangNameDisplayNode   {"languages.language.display_name"};
			static inline const string_t kLangAlphabetNode      {"languages.language.alphabet"};
			static inline const string_t kLangStructureNode     {"languages.language.structure"};
			static inline const string_t kWidgetNode            {"settings.widget"};
			static inline const string_t kWindow                {"settings.widget<Window>"};
			static inline const string_t kTextArea              {"settings.widget<TextArea>"};
			static inline const string_t kWinFontSizeNode       {"settings.widget<Window>.font.size"};
			static inline const string_t kWinFontNameNode       {"settings.widget<Window>.font.name"};
			static inline const string_t kWinBgColourNode       {"settings.widget<Window>.bgcolour"};
			static inline const string_t kWinFgColourNode       {"settings.widget<Window>.fgcolour"};
			static inline const string_t kWinBtnColourNode      {"settings.widget<Window>.btncolour"};
			static inline const string_t kWinWidthNode          {"settings.widget<Window>.width"};
			static inline const string_t kWinHeightNode         {"settings.widget<Window>.height"};
			static inline const string_t kAreaFontSizeNode      {"settings.widget<TextArea>.font.size"};
			static inline const string_t kAreaFontNameNode      {"settings.widget<TextArea>.font.name"};
			static inline const string_t kAreaBgColourNode      {"settings.widget<TextArea>.bgcolour"};
			static inline const string_t kAreaFgColourNode      {"settings.widget<TextArea>.fgcolour"};
			static inline const string_t kLogFile               {"settings.log.logfile"};
		};
		
		const string_t kDatabaseProtocol {"tcp://"};
		const string_t kDatabasePort {"3306"};
		const string_t kMysqlConnector {"MySQL"};
		const string_t kMariadbConnector {"MariaDB"};
		
		const string_t kPanelConfigPath {"/home/alvin/CLionProjects/Dixter/Resources/settings.xml"};
		const string_t kLangConfigPath  {"/home/alvin/CLionProjects/Dixter/Resources/languages.xml"};
		const string_t kVoiceConfigPath {"/home/alvin/CLionProjects/Dixter/Resources/voices.xml"};
	}
	
	static const string_t kPlaceholderColour {"#9b9b9b"};
	
	enum class WidgetID
	{
		None,
		Dictionary,
		Translator,
		Notebook,
		Grid,
		TextView,
		ControlPanel,
		SearchControl,
		ButtonLayout,
		WordBox,
		LANGBOX,
		ButtonCopy,
		ButtonClear,
		Panel,
		DictionaryPanel,
		TranslatorPanel,
		TranslatorLeft,
		TranslatorCentre,
		TranslatorRight,
		TranslatorAreaWest,
		TranslatorAreaEast,
		ButtonPanel,
		LangGrid,
		ButtonSpeakWest,
		ButtonTranslateWest,
		LangboxWest,
		LangboxEast,
		ButtonFlip,
		VoiceGrid,
		VoiceBoxT,
		ButtonSpeakEast,
		ButtonTranslateEast,
		VoiceLabel,
		Preferences,
		PrefToplevel
	};
	
	enum MenuID
	{
		IdMenuBar,
		IdFile,
		IdEdit,
		IdView,
		IdFileNew,
		IdFileOpen,
		IdFileQuit,
		IdEditCopy,
		IdEditPaste,
		IdEditPreferences,
		IdViewSort,
		IdViewSortName,
		IdViewSortType,
		IdViewSortDate
	};
}
