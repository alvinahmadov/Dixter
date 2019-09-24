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
#include "Constants.hpp"
#include "Gui/TextEdit.hpp"

namespace Dixter
{
	namespace Gui
	{
		TTextEdit::TTextEdit(QWidget* parent, const QString& placeholder,
		                     bool readOnly, int fontSize)
				: QTextEdit(parent),
				  m_fontSize(fontSize),
				  m_placeholderSet(false),
				  m_charCount(0),
				  m_placeholderText(placeholder)
		{
			auto textDocument = new QTextDocument(this);
			setAcceptRichText(false);
			setWordWrapMode(QTextOption::WordWrap);
			setDocument(textDocument);
			m_maxCharCount = static_cast<TSize>(size().height() / font().pointSize() *
					size().width() / font().pointSize());
			setPlaceholder();
			setFontPointSize(fontSize);
			setReadOnly(readOnly);
			connectEvents();
		}
		
		void TTextEdit::setPlaceholder()
		{
			if (not m_placeholderText.isEmpty() and not isPlaceholderSet())
			{
				m_placeholderSet = true;
				setPlaceholderText(m_placeholderText);
			}
			
			if (not document()->characterCount())
			{
				setFontItalic(true);
			}
		}
		
		void TTextEdit::resetPlaceholder()
		{
			if (isPlaceholderSet())
			{
				setFontItalic(false);
				m_placeholderSet = false;
			}
		}
		
		void TTextEdit::resizeEvent(QResizeEvent* sizeEvent)
		{
			m_maxCharCount = static_cast<TSize>(( sizeEvent->size().height() * sizeEvent->size().width()));
			document()->setPageSize(sizeEvent->size());
		}
		
		void TTextEdit::copyToClipboard(bool copyAll)
		{
			auto __currentPoint = cursor().pos();
			if (not copyAll)
			{
				copy();
			}
			else
			{
				selectAll();
				copy();
				cursor().setPos(__currentPoint);
			}
		}
		
		void TTextEdit::cutToClipboard(bool cutAll)
		{
			if (not cutAll)
			{
				cut();
			}
			else
			{
				selectAll();
				cut();
				setFocus();
			}
		}
		
		void TTextEdit::swapContent(TTextEdit* other)
		{
			if (not toPlainText().compare(other->toPlainText()))
				return;
			
			auto __thsVal = toPlainText();
			
			if (not other->isEmpty())
				setPlainText(other->toPlainText());
			
			if (not isEmpty())
				other->setPlainText(__thsVal);
		}
		
		QString TTextEdit::getContent() const
		{
			return document()->toPlainText();
		}
		
		void TTextEdit::write(const TTextEdit* other)
		{
			if (not other->isPlaceholderSet())
			{
				if (isPlaceholderSet())
					resetPlaceholder();
				
				if (not isEnabled()) textColor().setNamedColor(g_placeholderColour.c_str());
				setText(other->toPlainText());
			}
		}
		
		void TTextEdit::clearContent()
		{
			if (not isPlaceholderSet())
			{
				clearContent();
				setPlaceholder();
			}
		}
		
		bool TTextEdit::isPlaceholderSet() const
		{
			return m_placeholderSet;
		}
		
		bool TTextEdit::isEmpty() const
		{
			return document()->isEmpty();
		}
		
		void TTextEdit::connectEvents()
		{
			connect(this, SIGNAL(textChanged()), SLOT(resetPlaceholder()));
			connect(this, SIGNAL(textChanged()), SLOT(setPlaceholder()));
			connect(
					this, &QTextEdit::textChanged, [ & ]()
					{
						setFontPointSize(m_fontSize);
					});
		}
	}
}