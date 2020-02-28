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
		class TSettingsDialog : public QDialog
		{
		Q_OBJECT
		public:
			explicit TSettingsDialog(QWidget* parent = nullptr, const QString& title = "Settings");
			
			virtual ~TSettingsDialog() override;
			
			void init();
			
			void generalTab();
			
			void accept() override;
			
			void reject() override;
		
		private:
			QTabWidget* m_tabWidget;
			
			QDialogButtonBox* m_dialogButtonBox;
			
			QWidget* m_generalTab;
			
			QWidget* m_serverPanel;
		};
	}
}


