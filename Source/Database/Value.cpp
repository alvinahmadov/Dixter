/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Value.hpp"
#include "Macros.hpp"
#include "Exception.hpp"

namespace Dixter
{
	namespace Database
	{
		TValue::TValue(const TString& name, EDataType type, TSize size,
					   bool null, bool primaryKey, bool autoIncrement) noexcept
				: m_null(null),
				  m_primaryKey(primaryKey),
				  m_autoIncrement(autoIncrement),
				  m_size(size),
				  m_name(name),
				  m_type(type)
		{ }
		
		const TString&
		TValue::getValueName() const
		{
			return m_name;
		}
		
		const EDataType&
		TValue::getType() const
		{
			return m_type;
		}
		
		TString
		TValue::getTypeString() const
		{
			switch (m_type)
			{
				case EDataType::kBit:
					return xSTR(BIT);
				case EDataType::kTinyInt:
					return xSTR(TINYINT);
				case EDataType::kSmallInt:
					return xSTR(SMALLINT);
				case EDataType::kMediumInt:
					return xSTR(MEDIUMINT);
				case EDataType::kInteger:
					return xSTR(INTEGER);
				case EDataType::kBigInt:
					return xSTR(BIGINT);
				case EDataType::kReal:
					return xSTR(REAL);
				case EDataType::kNumeric:
					return xSTR(NUMERIC);
				case EDataType::kChar:
					return xSTR(CHAR);
				case EDataType::kBinary:
					return xSTR(BINARY);
				case EDataType::kVarChar:
					return xSTR(VARCHAR);
				case EDataType::kVarBinary:
					return xSTR(VARBINARY);
				case EDataType::kLongVarChar:
					return xSTR(LONGVARCHAR);
				case EDataType::kLongVarBinary:
					return xSTR(LONGVARBINARY);
				case EDataType::kTimeStamp:
					return xSTR(TIMESTAMP);
				case EDataType::kDate:
					return xSTR(DATE);
				case EDataType::kTime:
					return xSTR(TIME);
				case EDataType::kYear:
					return xSTR(YEAR);
				case EDataType::kDecimal:
					return xSTR(DECIMAL);
				case EDataType::kDouble:
					return xSTR(DOUBLE);
				case EDataType::kGeometry:
					return xSTR(GEOMETRY);
				case EDataType::kEnum:
					return xSTR(ENUM);
				case EDataType::kSet:
					return xSTR(SET);
				case EDataType::kSqlNull:
					return xSTR(SQLNULL);
				case EDataType::kJson:
					return xSTR(JSON);
				case EDataType::kTinyText:
					return xSTR(TINYTEXT);
				case EDataType::kText:
					return xSTR(TEXT);
				case EDataType::kMediumText:
					return xSTR(MEDIUMTEXT);
				case EDataType::kLongText:
					return xSTR(LONGTEXT);
				case EDataType::kInt:
					return xSTR(INT);
				case EDataType::kBool:
					return xSTR(BOOL);
				default:
					throw TNotFoundException("%s:%d Unknown type.\n", __FILE__, __LINE__);
			}
		}
		
		const TSize&
		TValue::getSize() const
		{
			return m_size;
		}
		
		bool TValue::isAutoIncrement() const
		{
			return m_autoIncrement;
		}
		
		bool TValue::isPrimaryKey() const
		{
			return m_primaryKey;
		}
		
		bool TValue::isNull() const
		{
			return m_null;
		}
	} // namespace Database
} // namespace Dixter