/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include <set>
#include <mutex>

#include "Commons.hpp"
#include "Utilities.hpp"

namespace sql
{
	class Driver;
	
	class ResultSet;
	
	class Connection;
	
	class PreparedStatement;
	
	class Statement;
} // namespace sql

namespace Dixter
{
	namespace Database
	{
		class TTable;
		
		class TValue;
		
		class TQueryBuilder;
		
		class TManager : public TNonCopyable
		{
		public:
			using TResultSetPtr = std::shared_ptr<sql::ResultSet>;
			using TPreparedStatementMap = std::map<TString, sql::PreparedStatement*>;
			using TTableMap = std::map<TString, TTable*>;
			#ifdef HAVE_CXX17
			using TClause = TStringView;
			#else
			using TClause = const TString&;
			#endif
		public:
			struct TConnectionManager
			{
				TConnectionManager() = delete;
				
				TConnectionManager(const TString& hostName, const TString& userName,
								   const TString& userPassword) noexcept;
				
				TConnectionManager(const TConnectionManager&) = default;
				
				TConnectionManager(TConnectionManager&&) = default;
				
				TConnectionManager& operator=(const TConnectionManager&) = default;
				
				TConnectionManager& operator=(TConnectionManager&&) = default;
				
				TString host_;
				
				TString name_;
				
				TString password_;
			};
		
		public:
			TManager(const TString& hostName, const TString& dbUser, const TString& dbPassword) noexcept;
			
			explicit TManager(const TConnectionManager& connectionManager) noexcept;
			
			~TManager() noexcept;
			
			void selectDatabase(const TString& database);
			
			void createTable(const TString& tableName,
							 const std::list<TValue*>& valueList, bool drop = true);
			
			void insertValues(const TString& tableName);
			
			std::vector<TString> getColumns(const TString& table);
			
			TResultSetPtr selectColumn(const TString& table,
									   const TString& column,
									   TClause clause = "");
			
			/**
			 *
			 * */
			TResultSetPtr selectColumns(const std::vector<TString>& tables,
										const std::vector<TString>& columns,
										UInt32 comparatorColumn,
										UInt32 leftTableIndex);
			
			TResultSetPtr selectColumnsWhere(const TString& table,
											 const std::vector<TString>& columns,
											 TClause clause);
			
			/**
			 *
			 * */
			TResultSetPtr selectColumns(const TString& table,
										const std::vector<TString>& columns,
										UInt32 comparatorColumn);
			
			/**
			 * Get values of columns matching regex value
			 * @param tables
			 * */
			TResultSetPtr selectColumnsLike(const std::vector<TString>& tables,
											const std::vector<TString>& columns,
											const TString& searchText,
											UInt32 comparatorColumn, UInt32 leftTableIndex,
											UInt32 fieldIndex);
			
			/**
			 *
			 * */
			void setBigInt(const TString& tableName, UInt32 parameterIndex,
						   const TString& value);
			
			void setBlob(const TString& tableName, UInt32 parameterIndex,
						 std::istream* blob);
			
			void setBoolean(const TString& tableName, UInt32 parameterIndex, bool value);
			
			void setDateTime(const TString& tableName, UInt32 parameterIndex,
							 const TString& value);
			
			void setDouble(const TString& tableName,
						   UInt32 parameterIndex, Real32 value);
			
			void setInt(const TString& tableName,
						UInt32 parameterIndex, Int32 value);
			
			void setUInt(const TString& tableName,
						 UInt32 parameterIndex, UInt32 value);
			
			void setInt64(const TString& tableName,
						  UInt32 parameterIndex, Int64 value);
			
			void setUInt64(const TString& tableName,
						   UInt32 parameterIndex, UInt64 value);
			
			void setNull(const TString& tableName,
						 UInt32 parameterIndex);
			
			void setString(const TString& tableName,
						   UInt32 parameterIndex, const TString& value);
			
			UInt32 setStrings(const TString& tableName, const UInt32& parameterStartIndex,
							  std::list<TString>& list, UInt32 stringColumnNum = 0);
			
			int executeUpdate(const TString& tableName);
		
		private:
			inline TResultSetPtr
			execute(const TString& query);
		
		private:
			sql::Driver* m_driver;
			
			sql::Connection* m_connection;
			
			sql::Statement* m_statement;
			
			TTableMap m_tables;
			
			TPreparedStatementMap m_prepStmts;
			
			TQueryBuilder* m_queryBuilder;
			
			TConnectionManager m_connManager;
			
			mutable std::mutex m_mutex;
		};
	} // namespace Database
} // namespace Dixter