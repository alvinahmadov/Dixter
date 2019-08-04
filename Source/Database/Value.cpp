/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Value.hpp"
#include "Commons.hpp"
#include "Macros.hpp"
#include "Utilities.hpp"


namespace Dixter
{
	namespace Database
	{
		Value::Value(const string_t& valueName, DataType valueType, size_t size, bool null, bool autoIncrement, bool primaryKey)
				: m_autoIncrement { autoIncrement },
				  m_null { null },
				  m_primaryKey { primaryKey },
				  m_size { size },
				  m_valueName { valueName },
				  m_valueType { valueType }
		{ }
		
		const string_t&
		Value::getValueName() const
		{
			return m_valueName;
		}
		
		const DataType&
		Value::getType() const
		{
			return m_valueType;
		}
		
		string_t
		Value::getTypeString() const
		{
			switch (m_valueType)
			{
				case DataType::kBit: return xSTR(BIT);
				case DataType::kTinyInt: return xSTR(TINYINT);
				case DataType::kSmallInt: return xSTR(SMALLINT);
				case DataType::kMediumInt: return xSTR(MEDIUMINT);
				case DataType::kInteger: return xSTR(INTEGER);
				case DataType::kBigInt: return xSTR(BIGINT);
				case DataType::kReal: return xSTR(REAL);
				case DataType::kNumeric: return xSTR(NUMERIC);
				case DataType::kChar: return xSTR(CHAR);
				case DataType::kBinary: return xSTR(BINARY);
				case DataType::kVarChar: return xSTR(VARCHAR);
				case DataType::kVarBinary: return xSTR(VARBINARY);
				case DataType::kLongVarChar: return xSTR(LONGVARCHAR);
				case DataType::kLongVarBinary: return xSTR(LONGVARBINARY);
				case DataType::kTimeStamp: return xSTR(TIMESTAMP);
				case DataType::kDate: return xSTR(DATE);
				case DataType::kTime: return xSTR(TIME);
				case DataType::kYear: return xSTR(YEAR);
				case DataType::kDecimal: return xSTR(DECIMAL);
				case DataType::kDouble: return xSTR(DOUBLE);
				case DataType::kGeometry: return xSTR(GEOMETRY);
				case DataType::kEnum: return xSTR(ENUM);
				case DataType::kSet: return xSTR(SET);
				case DataType::kSqlNull: return xSTR(SQLNULL);
				case DataType::kJson: return xSTR(JSON);
				case DataType::kTinyText: return xSTR(TINYTEXT);
				case DataType::kText: return xSTR(TEXT);
				case DataType::kMediumText: return xSTR(MEDIUMTEXT);
				case DataType::kLongText: return xSTR(LONGTEXT);
				case DataType::kInt: return xSTR(INT);
				case DataType::kBool: return xSTR(BOOL);
				default: throw NotFoundException { "%s:%d Unknown type.\n", __FILE__, __LINE__ };
			}
		}
		
		const size_t&
		Value::getSize() const
		{
			return m_size;
		}
		
		bool Value::isAutoIncrement() const
		{
			return m_autoIncrement;
		}
		
		bool Value::isNull() const
		{
			return m_null;
		}
		
		bool Value::isPrimaryKey() const
		{
			return m_primaryKey;
		}
	}
}