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
					return dxSTR(BIT);
				case EDataType::kTinyInt:
					return dxSTR(TINYINT);
				case EDataType::kSmallInt:
					return dxSTR(SMALLINT);
				case EDataType::kMediumInt:
					return dxSTR(MEDIUMINT);
				case EDataType::kInteger:
					return dxSTR(INTEGER);
				case EDataType::kBigInt:
					return dxSTR(BIGINT);
				case EDataType::kReal:
					return dxSTR(REAL);
				case EDataType::kNumeric:
					return dxSTR(NUMERIC);
				case EDataType::kChar:
					return dxSTR(CHAR);
				case EDataType::kBinary:
					return dxSTR(BINARY);
				case EDataType::kVarChar:
					return dxSTR(VARCHAR);
				case EDataType::kVarBinary:
					return dxSTR(VARBINARY);
				case EDataType::kLongVarChar:
					return dxSTR(LONGVARCHAR);
				case EDataType::kLongVarBinary:
					return dxSTR(LONGVARBINARY);
				case EDataType::kTimeStamp:
					return dxSTR(TIMESTAMP);
				case EDataType::kDate:
					return dxSTR(DATE);
				case EDataType::kTime:
					return dxSTR(TIME);
				case EDataType::kYear:
					return dxSTR(YEAR);
				case EDataType::kDecimal:
					return dxSTR(DECIMAL);
				case EDataType::kDouble:
					return dxSTR(DOUBLE);
				case EDataType::kGeometry:
					return dxSTR(GEOMETRY);
				case EDataType::kEnum:
					return dxSTR(ENUM);
				case EDataType::kSet:
					return dxSTR(SET);
				case EDataType::kSqlNull:
					return dxSTR(SQLNULL);
				case EDataType::kJson:
					return dxSTR(JSON);
				case EDataType::kTinyText:
					return dxSTR(TINYTEXT);
				case EDataType::kText:
					return dxSTR(TEXT);
				case EDataType::kMediumText:
					return dxSTR(MEDIUMTEXT);
				case EDataType::kLongText:
					return dxSTR(LONGTEXT);
				case EDataType::kInt:
					return dxSTR(INT);
				case EDataType::kBool:
					return dxSTR(BOOL);
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