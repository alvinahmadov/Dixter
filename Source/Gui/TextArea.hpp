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
		class TextArea : public QTextEdit
		{
		Q_OBJECT
		public:
			explicit TextArea(QWidget* parent, const QString& value = QString(),
			                  bool readOnly = false, int fontSize = 16);
			
			~TextArea() dxDECL_OVERRIDE;
			
			void copyToClipboard(bool copyAll = false);
			
			void cutToClipboard(bool cutAll = false);
			
			void swapContent(TextArea* other);
			
			QString getContent() const;
			
			void write(const TextArea* other);
			
			void clearContent();
			
			bool isPlaceholderSet() const;
			
			bool isEmpty() const;
		
		protected slots:
			void setPlaceholder();
			
			void resetPlaceholder();
		
		protected:
			void resizeEvent(QResizeEvent* sizeEvent) dxDECL_OVERRIDE;
			
			void connectEvents();
		
		private:
			int m_fontSize;
			
			bool m_placeholderSet;
			
			size_t m_maxCharCount;
			
			QString m_placeholderText;
			
			ui64 m_charCount;
		};
	}
}