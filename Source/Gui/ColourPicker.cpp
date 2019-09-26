/**
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */


#include "Macros.hpp"
#include "Gui/ColourPicker.hpp"

namespace Dixter
{
	namespace Gui
	{
		ColourPicker::ColourPicker(QWidget* parent, const QColor& colour, const QSize& size) noexcept
				: QColorDialog(parent)
		{
			setCurrentColor(colour);
			setFixedSize(size);
			connectEvents();
		}
		
		ColourPicker::~ColourPicker()
		{ }
		
		void ColourPicker::connectEvents()
		{ }
	} // namespace Gui
} // namespace Dixter