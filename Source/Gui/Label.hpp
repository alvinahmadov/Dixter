/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include <QLabel>

class QWidget;

namespace Dixter
{
	namespace Gui
	{
		class TLabel : public QLabel
		{
		Q_OBJECT
		public:
			explicit TLabel(const QString& label, QWidget* parent = nullptr, const QSize& size = QSize());
		
			explicit TLabel(const QString& label, const QSize& size = QSize(1, 1));
			
			~TLabel() noexcept override = default;
		};
	} // namespace Gui
} // namespace Dixter