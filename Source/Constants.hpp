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
		static const QString g_translatorName("Translator");
		static const QString g_dictionaryName("Dictionary");
		static const TString g_widgetGroup("widget");
		static const TString g_controlGroup("control");
		static const TString g_menuGroup("menu");
		static const TString g_layoutGroup("sizer");
		static const TString g_placeholderColour("#9b9b9b");
		static const TString g_searhPlaceholder("Search word...");
		
		static const TByte g_whiteSpace(' ');
		static const TByte g_comma(',');
		static const TByte g_endLine('\n');
		
		
		struct NodeKey final
		{
			static const TString kLangRoot, kVoiceRoot;
			static const TString kVoiceNode, kVoiceNameNode, kVoiceValueNode;
			static const TString kLangNode, kLangIdNode, kLangNameNode,
								 kLangNameDisplayNode, kLangAlphabetNode, kLangStructureNode;
			static const TString kWinFontSizeNode, kWinFontNameNode, kWinBgColourNode, kWinFgColourNode,
								 kWinBtnColourNode, kWinWidthNode, kWinHeightNode;
			static const TString kAreaFontSizeNode, kAreaFontNameNode, kAreaBgColourNode,
								 kAreaFgColourNode;
			
			static const TString kDatabaseHostNode, kDatabaseUserNode, kDatabasePassNode, kDatabasePortNode;
		};
		
		const TString g_dbProtocol("tcp://"),
					  g_dbConnector("MySQL"), g_mdbConnector("MariaDB");
		
		const TString g_indexTable("tables"), g_indexColumn("original_value");
		
		const TString g_guiConfigPath  ("/usr/local/share/dixter/Resources/settings.ini");
		const TString g_langConfigPath ("/usr/local/share/dixter/Resources/languages.xml");
		const TString g_voiceConfigPath("/usr/local/share/dixter/Resources/voices.xml");
		
		const TString NodeKey::kLangRoot("languages");
		const TString NodeKey::kVoiceRoot("voices");
		
		const TString NodeKey::kVoiceNode("voices.voice");
		const TString NodeKey::kVoiceNameNode("voices.voice.name");
		const TString NodeKey::kVoiceValueNode("voices.voice.value");
		const TString NodeKey::kLangNode("languages.language");
		const TString NodeKey::kLangIdNode("languages.language.id");
		const TString NodeKey::kLangNameNode("languages.language.name");
		const TString NodeKey::kLangNameDisplayNode("languages.language.display_name");
		const TString NodeKey::kLangAlphabetNode("languages.language.alphabet");
		const TString NodeKey::kLangStructureNode("languages.language.structure");
		
		const TString NodeKey::kWinWidthNode("window_width");
		const TString NodeKey::kWinHeightNode("window_height");
		const TString NodeKey::kWinBgColourNode("window_bgcolour");
		const TString NodeKey::kWinFgColourNode("window_fgcolour");
		const TString NodeKey::kWinBtnColourNode("window_btncolour");
		const TString NodeKey::kWinFontNameNode("window_font_name");
		const TString NodeKey::kWinFontSizeNode("window_font_size");
		const TString NodeKey::kAreaBgColourNode("textarea_bgcolour");
		const TString NodeKey::kAreaFgColourNode("textarea_fgcolour");
		const TString NodeKey::kAreaFontNameNode("textarea_font_name");
		const TString NodeKey::kAreaFontSizeNode("textarea_font_size");
		
		const TString NodeKey::kDatabaseHostNode("db_hostname");
		const TString NodeKey::kDatabaseUserNode("db_user");
		const TString NodeKey::kDatabasePassNode("db_password");
		const TString NodeKey::kDatabasePortNode("db_port");
	} // anonymous namespace
	
	enum class EWidgetID
	{
		None = -1,
		Dictionary,
		Translator,
		Notebook,
		TextView,
		Grid,
		LangGrid,
		VoiceGrid,
		SearchControl,
		ButtonLayout,
		WordBox, LangBox, LangboxWest, LangboxEast,
		ButtonCopy, ButtonClear,
		ButtonSpeakWest, ButtonTranslateWest, ButtonFlip, ButtonSpeakEast, ButtonTranslateEast,
		Panel, DictionaryPanel, TranslatorPanel, ControlPanel, ButtonPanel,
		TranslatorLeft, TranslatorCentre, TranslatorRight,
		TranslatorAreaWest, TranslatorAreaEast,
		VoiceBoxT, VoiceLabel, Preferences
	};
	
	enum class EMenuID
	{
		IdMenuBar,
		IdFile, IdEdit, IdView,
		IdFileNew, IdFileOpen, IdFileQuit,
		IdEditCopy, IdEditPaste, IdEditPreferences,
		IdViewSort, IdViewSortName, IdViewSortType, IdViewSortDate
	};
} // namespace Dixter
