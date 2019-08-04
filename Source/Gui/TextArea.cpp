/**
 *  File Textarea.cpp
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <QSize>
#include <QDebug>
#include <QResizeEvent>

#include "Configuration.hpp"
#include "Gui/TextArea.hpp"

namespace Dixter
{
	namespace Gui
	{
		TextArea::TextArea(QWidget* parent, const QString& placeholder,
		                   bool readOnly, int fontSize)
				: QTextEdit(parent),
				  m_fontSize { fontSize },
				  m_placeholderSet { false },
				  m_placeholderText { placeholder },
				  m_charCount { }
		{
			auto textDocument = new QTextDocument(this);
			setAcceptRichText(false);
			setWordWrapMode(QTextOption::WordWrap);
			setDocument(textDocument);
			m_maxCharCount = static_cast<size_t>(size().height() / font().pointSize() *
			                                     size().width() / font().pointSize());
			setPlaceholder();
			setFontPointSize(fontSize);
			setReadOnly(readOnly);
			connectEvents();
		}
		
		TextArea::~TextArea()
		{ }
		
		void TextArea::setPlaceholder()
		{
			if (!m_placeholderText.isEmpty() && !isPlaceholderSet())
			{
				m_placeholderSet = true;
				
				setPlaceholderText(m_placeholderText);
			}
			
			if (!document()->characterCount())
			{
				setFontItalic(true);
			}
		}
		
		void TextArea::resetPlaceholder()
		{
			if (isPlaceholderSet())
			{
				setFontItalic(false);
				m_placeholderSet = false;
			}
		}
		
		void TextArea::resizeEvent(QResizeEvent* sizeEvent)
		{
			m_maxCharCount = static_cast<size_t>((sizeEvent->size().height() * sizeEvent->size().width()));
			document()->setPageSize(sizeEvent->size());
		}
		
		void TextArea::copyToClipboard(bool copyAll)
		{
			
			auto __currentPoint = cursor().pos();
			if (copyAll)
			{
				selectAll();
				copy();
				cursor().setPos(__currentPoint);
			} else copy();
			
		}
		
		void TextArea::cutToClipboard(bool cutAll)
		{
			if (cutAll)
			{
				selectAll();
				cut();
				setFocus();
			} else cut();
		}
		
		void TextArea::swapContent(TextArea* other)
		{
			if (!toPlainText().compare(other->toPlainText()))
				return;
			
			auto __thsVal = toPlainText();

			if (!other->isEmpty())
			{
				setPlainText(other->toPlainText());
			}
			if (!isEmpty())
			{
				other->setPlainText(__thsVal);
			}
		}
		
		QString TextArea::getContent() const
		{
			return document()->toPlainText();
		}
		
		void TextArea::write(const TextArea* other)
		{
			if (!other->isPlaceholderSet())
			{
				if (isPlaceholderSet()) resetPlaceholder();
				
				if (!isEnabled()) textColor().setNamedColor(kPlaceholderColour.c_str());
				setText(other->toPlainText());
			}
		}
		
		void TextArea::clearContent()
		{
			if (!isPlaceholderSet())
			{
				clearContent();
				setPlaceholder();
			}
		}
		
		bool TextArea::isPlaceholderSet() const
		{
			return m_placeholderSet;
		}
		
		bool TextArea::isEmpty() const
		{
			return document()->isEmpty();
		}
		
		void TextArea::connectEvents()
		{
			connect(this, SIGNAL(textChanged()), SLOT(resetPlaceholder()));
			connect(this, SIGNAL(textChanged()), SLOT(setPlaceholder()));
			connect(this, &QTextEdit::textChanged, [&]()
			{
				setFontPointSize(m_fontSize);
			});
		}
	}
}