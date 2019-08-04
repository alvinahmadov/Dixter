/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#ifndef _DIXTER_DBMANAGER_H_
#define _DIXTER_DBMANAGER_H_


#include <set>

#include "Commons.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"


namespace sql
{
	class Driver;
	
	class ResultSet;
	
	class Connection;
	
	class PreparedStatement;
	
	class Statement;
}

namespace Dixter
{
	namespace Database
	{
		class Table;
		
		class Value;
		
		class QueryBuilder;
		
		struct ConnectionManager
		{
			ConnectionManager(const string_t& hostname, const string_t& database,
			                  const string_t& username, const string_t& password,
			                  const string_t& port)
					: _host { Utilities::Strings::concat(kDatabaseProtocol, hostname, ":", port) },
					  _database { database },
					  _name { username },
					  _password { password }
			{ }
			
			string_t _host;
			
			string_t _database;
			
			string_t _name;
			
			string_t _password;
		};
		
		class Manager
		{
		public:
			Manager(const string_t& host, const string_t& database,
			        const string_t& userName, const string_t& password,
			        const string_t& port = kDatabasePort);
			
			Manager(const ConnectionManager& connectionManager);
			
			~Manager();
			
			void createTable(const string_t& tableName,
			                 const std::list<Value*>& valueList, bool drop = true);
			
			void insertValues(const string_t& tableName);
			
			sql::ResultSet* selectColumn(const string_t& table,
			                             const string_t& column,
			                             string_t clause = "");
			
			std::vector<string_t> getColumns(const string_t& table);
			
			/**
			 *
			 * */
			sql::ResultSet* selectColumns(const std::vector<string_t>& tables,
			                              const std::vector<string_t>& columns,
			                              ui32 comparatorColumn,
			                              ui32 leftTableIndex);
			
			sql::ResultSet* selectColumnsWhere(const string_t& table,
			                                   const std::vector<string_t>& columns,
			                                   string_t clause);
			
			/**
			 *
			 * */
			sql::ResultSet* selectColumns(const string_t& table,
			                              const std::vector<string_t>& columns,
			                              ui32 comparatorColumn);
			
			/**
			 * Get values of columns matching regex value
			 * @param tables
			 * */
			sql::ResultSet* selectColumnsLike(const std::vector<string_t>& tables,
			                                  const std::vector<string_t>& columns,
			                                  const string_t& searchText,
			                                  ui32 comparatorColumn, ui32 leftTableIndex,
			                                  ui32 fieldIndex);
			
			/**
			 *
			 * */
			void setBigInt(const string_t& tableName, ui32 parameterIndex,
			               const string_t& value);
			
			void setBlob(const string_t& tableName, ui32 parameterIndex,
			             std::istream* blob);
			
			void setBoolean(const string_t& tableName, ui32 parameterIndex, bool value);
			
			void setDateTime(const string_t& tableName, ui32 parameterIndex,
			                 const string_t& value);
			
			void setDouble(const string_t& tableName,
			               ui32 parameterIndex, d32 value);
			
			void setInt(const string_t& tableName,
			            ui32 parameterIndex, i32 value);
			
			void setUInt(const string_t& tableName,
			             ui32 parameterIndex, ui32 value);
			
			void setInt64(const string_t& tableName,
			              ui32 parameterIndex, i64 value);
			
			void setUInt64(const string_t& tableName,
			               ui32 parameterIndex, ui64 value);
			
			void setNull(const string_t& tableName,
			             ui32 parameterIndex);
			
			void setString(const string_t& tableName,
			               ui32 parameterIndex, const string_t& value);
			
			ui32 setStrings(const string_t& tableName, const ui32& parameterStartIndex,
			                std::list<string_t>& list, ui32 stringColumnNum = 0);
			
			int executeUpdate(const string_t& tableName);
			
			sql::ResultSet* execute(const string_t& tableName,
			                        string_t query);
		
		private:
			sql::Driver* m_driver;
			
			sql::Connection* m_connection;
			
			sql::Statement* m_statement;
			
			std::map<string_t, Table*>* m_tables;
			
			std::map<string_t, sql::PreparedStatement*>* m_preparedStmts;
			
			QueryBuilder* m_queryBuilder;
			
			ConnectionManager* m_connectionManager;
		};
		
	}
}

#endif //_DATA_BUILDER_H_
