/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QResizeEvent>

#include "Gui/Notebook.hpp"
#include "Gui/Panel.hpp"

namespace Dixter
{
	namespace Gui
	{
		TNotebook::TNotebook(QWidget* parent, const QString& name)
				: QTabWidget(parent)
		{
			setObjectName(name);
			connectEvents();
		}
		
		void TNotebook::addPage(QWidget* page, const QString& title)
		{
			addTab(page, title);
		}
		
		void TNotebook::resizeEvent(QResizeEvent* resizeEvent)
		{
			for (int index = 0; index < count(); ++index)
				widget(index)->setFixedWidth(resizeEvent->size().width());
		}
		
		void TNotebook::connectEvents()
		{ }
		
		APanel*
		TNotebook::getPage(int pageNum)
		{
			int __index {};
			for (const auto* __child : children())
			{
				if (__index == pageNum)
					return const_cast<APanel*>(static_cast<const APanel*>(__child));
				++__index;
			}
			return nullptr;
		}
		
		int TNotebook::getPageNumber(QWidget*) const
		{ }
	} // namespace Gui
} // namespace Dixter