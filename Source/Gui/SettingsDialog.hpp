/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QDialog>

#include "Types.hpp"
#include "Group.hpp"

class QWidget;
class QTabWidget;
class QDialogButtonBox;
class QWidget;
class QGridLayout;
class QDialogButtonBox;
class QFormLayout;

namespace Dixter
{
	class SettingsController;
	namespace Gui
	{
		class SettingsDialog : public QDialog
		{
		Q_OBJECT
		public:
			explicit SettingsDialog(QWidget* parent = nullptr, const QString& title = "Settings");
			
			void init();
			
			void generalTab();
			
			void accept();
			
			void reject();
		
		private:
			QTabWidget* m_tabWidget;
			
			QDialogButtonBox* m_dialogButtonBox;
			
			QWidget* m_generalTab;
			
			QWidget* m_serverPanel;
		};
	}
}


