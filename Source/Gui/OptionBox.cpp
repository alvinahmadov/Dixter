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
#include "Utilities.hpp"
#include "Configuration.hpp"
#include "Gui/OptionBox.hpp"

namespace AlgoUtils = Dixter::Utilities::Algorithms;
using std::vector;

namespace Dixter
{
	namespace Gui
	{
		void arrayStringToVector(const QStringList& src, vector<QString>& dest)
		{
			AlgoUtils::foreachCompound(src, dest, [](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			});
		}
		
		void vectorToArrayString(const vector<QString>& src, QStringList& dest)
		{
			Utilities::Algorithms::foreachCompound(src, dest, [ ](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			});
		}
		
		QStringList vectorToArrayString(const vector<QString>& src)
		{
			QStringList __ret {};
	
			Utilities::Algorithms::foreachCompound(src, __ret, [ ](const QString& srcValue, QString& destValue)
			{
				destValue = srcValue;
			});
			return __ret;
		}
		
		Int32 compare(const QVariant& v1, const QVariant& v2)
		{
			return v1.toString().compare(v2.toString());
		}
		
		TOptionBox::TOptionBox(QWidget* parent,
							   const QString& placeholder,
							   const QSize& size, bool sort)
				: QComboBox(parent),
				  m_isPlaceholderSet { },
				  m_sort { sort },
				  m_placeHolder { placeholder }
		{
			setPlaceholder(placeholder);
			if (size.height() > 0 and size.width() > 0)
				setMinimumSize(size);
			
			connectEvents();
		}
		
		TOptionBox::TOptionBox(const QString& placeholder,
							   const QSize& size, bool sort)
				: TOptionBox(nullptr, placeholder, size, sort)
		{ }
		
		inline void TOptionBox::setPlaceholder(const QString& placeholder)
		{
			if (not placeholder.isEmpty())
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
		
		void TOptionBox::resetPlaceholder()
		{
			if (m_isPlaceholderSet)
			{
				auto idx = findText(m_placeHolder);
				if (idx > -1)
					removeItem(idx);
				m_isPlaceholderSet = false;
			}
		}
		
		void TOptionBox::setValues(vector<TUString>& options, bool sort)
		{
			Q_UNUSED(sort)
			
			if (options.size() > 0)
			{
				for (const auto& option : options)
				{
					auto __option = option.asCustom();
					addItem(__option);
				}
			}
		}
		
		void TOptionBox::setValues(const QStringList& options)
		{
			if (options.size() > 0)
			{
				for (const auto& option : options)
				{
					addItem(option);
				}
			}
		}
		
		void TOptionBox::swapCurrent(TOptionBox* src)
		{
			if(isPlaceholderSet() or src->isPlaceholderSet())
				return;
			
			auto __srcCount = src->getItemCount();
			auto __currentIndex = currentIndex();
			
			if (__srcCount > 0)
			{
				setCurrentIndex(src->currentIndex());
				src->setCurrentIndex(__currentIndex);
			}
		}
		
		void TOptionBox::sort(bool ascending)
		{
			//TODO(Alvin): Reimplement
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
		
		Int32 TOptionBox::getItemCount() const
		{
			return (isPlaceholderSet() ? count() - 1 : count());
		}
		
		Int32 TOptionBox::getPosition(const QString& value)
		{
			Int32 __pos { -1 };
			
			__pos = findText(value);
			return __pos;
		}
		
		bool TOptionBox::isPlaceholderSet() const
		{
			return m_isPlaceholderSet;
		}
		
		void TOptionBox::onChanged(int)
		{
			resetPlaceholder();
		}
		
		void TOptionBox::connectEvents()
		{
			connect(this, SIGNAL(activated(int)), SLOT(onChanged(int)));
		}
	}
}
