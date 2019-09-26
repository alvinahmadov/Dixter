/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include "Button.hpp"
#include "Constants.hpp"
#include "Configuration.hpp"

namespace Dixter
{
	namespace Gui
	{
		TButton::TButton(QWidget* parent, const QString& label)
				: QPushButton(label, parent)
		{
			init();
		}
		
		TButton::TButton(QIcon icon, QWidget* parent)
				: TButton(parent, QString())
		{
			icon.actualSize(size());
			setIcon(icon);
		}
		
		TButton::TButton(const QString& label)
				: TButton(nullptr, label)
		{
		}
		
		void TButton::init()
		{
			auto __confMan = getIniManager({ g_guiConfigPath })->accessor();
			auto __bgColour = __confMan->getValue(NodeKey::kWinBtnColourNode).asCustom();
			auto __fontName = __confMan->getValue(NodeKey::kWinFontNameNode).asCustom();
			int __fontSize = __confMan->getValue(NodeKey::kWinFontSizeNode);
			__bgColour.prepend('#');
			QPalette __palette(__bgColour);
			
			setPalette(__palette);
			setFont(QFont(__fontName, __fontSize));
		}
	} // namespace Gui
} //namespace Dixter