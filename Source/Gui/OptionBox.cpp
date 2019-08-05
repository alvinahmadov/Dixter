/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#include <QDebug>

#include "Commons.hpp"
#include "Configuration.hpp"
#include "Gui/OptionBox.hpp"

using namespace Dixter::Utilities;
using std::vector;

namespace Dixter
{
	namespace Gui
	{
		void arrayStringToVector(const QStringList& src, vector<QString>& dest)
		{
			auto fCopier = [ ](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			};
			
			Algorithms::foreachCompound(src, dest, fCopier);
		}
		
		void vectorToArrayString(const vector<QString>& src, QStringList& dest)
		{
			auto fCopier = [ ](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			};
			
			Utilities::Algorithms::foreachCompound(src, dest, fCopier);
		}
		
		QStringList vectorToArrayString(const vector<QString>& src)
		{
			auto __ret = QStringList();
			auto fCopier = [ ](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			};
			
			Utilities::Algorithms::foreachCompound(src, __ret, fCopier);
			return __ret;
		}
		
		int compare(const QVariant& v1, const QVariant& v2)
		{
			return v1.toString().compare(v2.toString());
		}
		
		OptionBox::OptionBox(QWidget* parent,
							 const QString& placeholder,
							 const QSize& size, bool sort)
				: QComboBox(parent),
				  m_isPlaceholderSet { },
				  m_sort { sort },
				  m_placeHolder { placeholder }
		{
			setPlaceholder(placeholder);
			if (size.height() > 0 && size.width() > 0)
				setMinimumSize(size);
			
			connectEvents();
		}
		
		OptionBox::OptionBox(const QString& placeholder,
							 const QSize& size, bool sort)
				: OptionBox(nullptr, placeholder, size, sort)
		{
		
		}
		
		OptionBox::~OptionBox()
		{ }
		
		inline void OptionBox::setPlaceholder(const QString& placeholder)
		{
			// if (!placeholder.isEmpty())
			{
				m_placeHolder = placeholder;
				m_isPlaceholderSet = true;
				
				if (count() > 0)
				{
					int srchNum { };
					srchNum = findText(m_placeHolder);
					if (srchNum != -1)
					{
						setCurrentIndex(srchNum);
					} else
					{
						insertItem(0, m_placeHolder);
					}
				} else
				{
					addItem(m_placeHolder);
				}
				setCurrentText(m_placeHolder);
			}
		}
		
		void OptionBox::resetPlaceholder()
		{
			if (m_isPlaceholderSet)
			{
				auto idx = findText(m_placeHolder);
				if (idx > -1)
					removeItem(idx);
				m_isPlaceholderSet = false;
			}
		}
		
		void OptionBox::setValues(vector<ustring_t>& options, bool sort)
		{
			Q_UNUSED(sort)
			// if (sort)
			// {
			// 	std::sort(options.begin(), options.end(), std::less<ustring_t>());
			// }
			
			if (options.size() > 0)
			{
				for (const auto& option : options)
				{
					auto __option = option.asCustom();
					addItem(__option);
				}
			}
		}
		
		void OptionBox::setValues(const QStringList& options)
		{
			if (options.size() > 0)
			{
				for (const auto& option : options)
				{
					addItem(option);
				}
			}
		}
		
		void OptionBox::swapCurrent(OptionBox* src)
		{
			if(isPlaceholderSet() || src->isPlaceholderSet())
				return;
			
			auto __srcCount = src->getItemCount();
			auto __currentIndex = currentIndex();
			
			if (__srcCount > 0)
			{
				setCurrentIndex(src->currentIndex());
				src->setCurrentIndex(__currentIndex);
			}
		}
		
		void OptionBox::sort(bool ascending)
		{
			//TODO: Reimplement
			(void)ascending;
			// auto __currentValues = currentData();
			// clear();
			// if (ascending)
			// {
			// 	std::sort(__currentValues.begin(), __currentValues.end(), std::greater<QString>());
			// } else
			// {
			// 	std::sort(__currentValues.begin(), __currentValues.end(), std::less<QString>());
			// }
			//
			// setValues(__currentValues);
		}
		
		int OptionBox::getItemCount() const
		{
			return (isPlaceholderSet() ? count() - 1 : count());
		}
		
		int OptionBox::getPosition(const QString& value)
		{
			int __pos { -1 };
			
			__pos = findText(value);
			return __pos;
		}
		
		bool OptionBox::isPlaceholderSet() const
		{
			return m_isPlaceholderSet;
		}
		
		void OptionBox::onChanged(int)
		{
			resetPlaceholder();
		}
		
		void OptionBox::connectEvents()
		{
			connect(this, SIGNAL(activated(int)), SLOT(onChanged(int)));
		}
	}
}
