/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QTabWidget>

#include "Configuration.hpp"

class QWidget;
class QResizeEvent;

namespace Dixter
{
	namespace Gui
	{
		class Panel;
		class Notebook : public QTabWidget
		{
		Q_OBJECT
		public:
			Notebook(QWidget* parent = nullptr, const QString& name = "");
			
			~Notebook() dxDECL_OVERRIDE;
			
			void addPage(QWidget* page, const QString& title);
			
			Panel* getPage(int pageNumber);
			
			int getPageNumber(QWidget* page) const;
		
		protected slots:
			void resizeEvent(QResizeEvent *) override;
			
		protected:
			void connectEvents();
		};
	}
}