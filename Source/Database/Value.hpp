/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include "Types.hpp"

namespace Dixter
{
	namespace Database
	{
		enum class EDataType
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
		
		class TValue
		{
		public:
			TValue(const TString& name, EDataType type, TSize size = 0, bool null = true,
				   bool primaryKey = false, bool autoIncrement = false) noexcept;
			
			TValue(const TValue&) = default;
			
			TValue& operator=(const TValue&) = default;
			
			TValue(TValue&&) = default;
			
			TValue& operator=(TValue&&) = default;
			
			const TString& getValueName() const;
			
			const EDataType& getType() const;
			
			TString getTypeString() const;
			
			const TSize& getSize() const;
			
			bool isAutoIncrement() const;
			
			bool isPrimaryKey() const;
			
			bool isNull() const;
		
		private:
			bool m_null;
			
			bool m_primaryKey;
			
			bool m_autoIncrement;
			
			TSize m_size;
			
			TString m_name;
			
			EDataType m_type;
		};
	} // namespace Database
} // namespace Dixter