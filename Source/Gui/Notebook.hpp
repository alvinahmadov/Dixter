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
#include <memory>

class QWidget;
class QResizeEvent;

namespace Dixter
{
	namespace Gui
	{
		class TPanel;
		
		class TNotebook : public QTabWidget
		{
		Q_OBJECT
		public:
			TNotebook(QWidget* parent = nullptr, const QString& name = "");
			
			~TNotebook() override = default;
			
			void addPage(QWidget* page, const QString& title);
			
			TPanel*
			getPage(int pageNumber);
			
			int getPageNumber(QWidget* page) const;
		
		protected slots:
			void resizeEvent(QResizeEvent *) override;
			
		protected:
			void connectEvents();
		};
	} // namespace Gui
} // namespace Dixter