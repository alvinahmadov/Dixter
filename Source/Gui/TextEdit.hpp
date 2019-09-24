/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <QTextEdit>

#include "Commons.hpp"
#include "Types.hpp"

namespace Dixter
{
	namespace Gui
	{
		class TTextEdit : public QTextEdit
		{
		Q_OBJECT
		public:
			TTextEdit(QWidget* parent, const QString& value = QString(),
			          bool readOnly = false, int fontSize = 16);
			
			virtual ~TTextEdit() noexcept override = default;
			
			void copyToClipboard(bool copyAll = false);
			
			void cutToClipboard(bool cutAll = false);
			
			void swapContent(TTextEdit* other);
			
			QString getContent() const;
			
			void write(const TTextEdit* other);
			
			void clearContent();
			
			bool isPlaceholderSet() const;
			
			bool isEmpty() const;
		
		protected slots:
			void setPlaceholder();
			
			void resetPlaceholder();
		
		protected:
			void resizeEvent(QResizeEvent* sizeEvent) override;
			
			void connectEvents();
		
		private:
			int m_fontSize;
			
			bool m_placeholderSet;
			
			TSize m_maxCharCount;
			
			UInt64 m_charCount;
			
			QString m_placeholderText;
		};
	}
}