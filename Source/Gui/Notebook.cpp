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
		Notebook::Notebook(QWidget* parent, const QString& name)
				: QTabWidget(parent)
		{
			setObjectName(name);
			connectEvents();
		}
		
		Notebook::~Notebook()
		{ }
		
		void Notebook::addPage(QWidget* page, const QString& title)
		{
			addTab(page, title);
		}
		
		void Notebook::resizeEvent(QResizeEvent *resizeEvent)
		{
			for (int index = 0; index < count(); ++index)
			{
				widget(index)->setFixedWidth(resizeEvent->size().width());
			}
		}
		
		void Notebook::connectEvents()
		{
		}
		
		Panel* Notebook::getPage(int pageNum)
		{
			int index { };
			for (const auto* child : children())
			{
				if (index == pageNum)
				{
					return const_cast<Panel*>(static_cast<const Panel*>(child));
				}
				++index;
			}
		}
		
		int Notebook::getPageNumber(QWidget*) const
		{ }
	}
}