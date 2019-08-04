/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QTabWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>

#include "SettingsController.hpp"
#include "Utilities.hpp"

#include "Gui/SettingsDialog.hpp"
#include "Gui/ColourPicker.hpp"
#include "Gui/FontPicker.hpp"
#include "Gui/OptionBox.hpp"
#include "Gui/Button.hpp"

namespace Dixter
{
	namespace Gui
	{
		SettingsDialog::SettingsDialog(QWidget* parent, const QString& title)
				: QDialog(parent),
				  m_tabWidget { new QTabWidget(this) },
				  m_generalTab { new QWidget(this) },
				  m_serverPanel { new QWidget(this) }
		{
			init();
			setWindowTitle(title);
			setFixedSize(800, 800);
		}
		
		void SettingsDialog::init()
		{
			setFixedSize(QSize(800, 800));
			setModal(true);
			generalTab();
			
			m_tabWidget->addTab(m_generalTab, tr("&General"));
			m_tabWidget->addTab(m_serverPanel, tr("&Server Backend"));
			
			m_tabWidget->show();
			m_dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			                                         Qt::Horizontal, this);
			
			connect(m_dialogButtonBox, SIGNAL(accepted()), SLOT(accept()));
			connect(m_dialogButtonBox, SIGNAL(rejected()), SLOT(reject()));
			
			setLayout(new QGridLayout(this));
			layout()->addWidget(m_tabWidget);
			layout()->addWidget(m_dialogButtonBox);
		}
		
		void SettingsDialog::generalTab()
		{
			m_generalTab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
			auto loginBox = new QGroupBox("Login", m_generalTab);
			
			loginBox->setFixedWidth(width());
			
			auto loginLayout = new QHBoxLayout();
			loginLayout->setObjectName("loginLayout");
			loginLayout->setContentsMargins(60, 30, 60, 30);
			loginLayout->setSizeConstraint(QFormLayout::SetNoConstraint);
			// Forms
			auto inputLayout = new QFormLayout();
			m_generalTab->setBackgroundRole(QPalette::ColorRole::Background);
			inputLayout->setObjectName("m_credentialsLayout");
			
			auto emailLabel = new QLabel(tr("Email: "), loginBox);
			auto emailInput = new QLineEdit(loginBox);
			auto passwordLabel = new QLabel(tr("Password: "), loginBox);
			auto passwordInput = new QLineEdit(loginBox);
			auto tokenLabel = new QLabel(tr("Token: "), loginBox);
			auto tokenInput = new QLineEdit(loginBox);
			
			// Echo modes
			passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
			tokenInput->setEchoMode(QLineEdit::EchoMode::Password);
			
			inputLayout->addRow(emailLabel, emailInput);
			inputLayout->addRow(passwordLabel, passwordInput);
			inputLayout->addRow(tokenLabel, tokenInput);
			
			// Default/Placeholder values
			// emailInput->setText(g_settingsManager->email());
			// passwordInput->setText(g_settingsManager->password());
			tokenInput->setPlaceholderText("Optional");
			
			// auto apiLayout = new Q
			
			// Connections
			// connect(emailInput, SIGNAL(textEdited(
			// 		                           const QString&)),
			//         g_settingsManager, SLOT(setEmail(const QString&)));
			//
			// connect(passwordInput, SIGNAL(textEdited(const QString&)),
			//         g_settingsManager, SLOT(setPassword(const QString&)));
			//
			// connect(tokenInput, SIGNAL(textEdited(const QString&)),
			//         g_settingsManager, SLOT(setToken(const QString&)));
			
			// Finishing
			loginLayout->addLayout(inputLayout);
			loginBox->setLayout(loginLayout);
		}
		
		void SettingsDialog::accept()
		{
			// g_settingsManager->save();
			QDialog::accept();
		}
		
		void SettingsDialog::reject()
		{
			QDialog::reject();
		}
	}
}