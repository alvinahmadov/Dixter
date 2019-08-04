/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once


#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "Types.hpp"


namespace Dixter
{
	namespace Database
	{
		enum class DataType
		{
			kUnknown = 0,
			kBit,
			kTinyInt,
			kSmallInt,
			kMediumInt,
			kInteger,
			kBigInt,
			kReal,
			kDouble,
			kDecimal,
			kNumeric,
			kChar,
			kBinary,
			kVarChar,
			kVarBinary,
			kLongVarChar,
			kLongVarBinary,
			kTimeStamp,
			kDate,
			kTime,
			kYear,
			kGeometry,
			kEnum,
			kSet,
			kSqlNull,
			kJson,
			kTinyText,
			kText,
			kMediumText,
			kLongText,
			kInt,
			kBool
		};
		
		class Value
		{
		public:
			Value(const string_t& valueName, DataType valueType, size_t size = 0, bool null = true,
			      bool autoIncrement = false, bool primaryKey = false);
			
			const string_t& getValueName() const;
			
			const DataType& getType() const;
			
			string_t getTypeString() const;
			
			const size_t& getSize() const;
			
			bool isAutoIncrement() const;
			
			bool isNull() const;
			
			bool isPrimaryKey() const;
		
		private:
			bool m_autoIncrement;
			
			bool m_null;
			
			bool m_primaryKey;
			
			size_t m_size;
			
			string_t m_valueName;
			
			DataType m_valueType;
		};
	}
}