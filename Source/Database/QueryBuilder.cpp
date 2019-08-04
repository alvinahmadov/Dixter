
#include "QueryBuilder.hpp"
#include "Utilities.hpp"

namespace Dixter
{
	namespace Database
	{
		QueryBuilder::QueryBuilder()
				: m_query {}
		{}
		
		QueryBuilder::~QueryBuilder()
		{}
		
		const string_t&
		QueryBuilder::describeQuery(const string_t& tableName)
		{
			m_query.clear();
			m_query = "DESCRIBE " + tableName;
			return m_query;
		}
		
		const string_t&
		QueryBuilder::dropQuery(const string_t& tableName)
		{
			//if (!m_query.empty())
			m_query.clear();
			m_query = "DROP TABLE IF EXISTS " + tableName;
			return m_query;
		}
		
		const string_t&
		QueryBuilder::createQuery(const string_t& tableName, const std::list<Value*>& dbValueList, size_t& parametersNum)
		{
			string_t primaryKey {};
			m_query.clear();
			
			m_query += "CREATE TABLE " + tableName + " (";
			for (const auto& value : dbValueList)
			{
				bool autoInc {value->isAutoIncrement()};
				bool null {value->isNull()};
				bool primary {value->isPrimaryKey()};
				m_query += " " + value->getValueName() + " " + value->getTypeString();
				if (value->getSize() != 0)
					m_query += " (" + std::to_string(value->getSize()) + ") ";
				if (autoInc)
					m_query += " AUTO_INCREMENT";
				if (null)
					m_query += " NULL";
				else
					m_query += " NOT NULL";
				if (primary)
					primaryKey += ", PRIMARY KEY (" + value->getValueName() + ")";
				++parametersNum;
				if (parametersNum != dbValueList.size())
					m_query.push_back(',');
			}
			m_query += primaryKey + ')';
			return m_query;
		}
		
		const string_t&
		QueryBuilder::insertQuery(const string_t& tableName, const size_t& parametersNum)
		{
			string_t params {};
			size_t index {};
			m_query.clear();
			m_query += "INSERT INTO " + tableName + " VALUES (";
			Utilities::Algorithms::forEach(index, parametersNum, [ & ](size_t&)
			{
				params += '?';
				if (index != parametersNum - 1)
					params += ", ";
			});
			m_query += params + ')';
			return m_query;
		}
		
		const string_t&
		QueryBuilder::selectQuery(const std::vector<string_t>& tables, const std::vector<string_t>& columns,
		                          ui32 indexColumn, ui32 leftTableIndex)
		{
			m_query.clear();
			m_query += "SELECT ";
			
			for (ui32 index = 0; index < columns.size(); index++)
			{
				if (index != indexColumn)
					m_query.append(columns.at(index));
				if (index != columns.size() - 1 && index != indexColumn)
					m_query.push_back(',');
				m_query.push_back(' ');
			}
			m_query += " FROM " + tables.at(leftTableIndex);
			if (tables.size() > 1)
			{
				m_query += " JOIN (";
				for (ui32 i = 0; i < tables.size(); i++)
				{
					if (i != leftTableIndex)
						m_query += tables.at(i);
					
					if (i != tables.size() - 1 && i != leftTableIndex)
						m_query += ", ";
				}
				m_query += ") ON ";
			}
			for (ui32 i {}; i < tables.size(); i++)
			{
				if (tables.at(leftTableIndex) != tables.at(i) && leftTableIndex != i)
				{
					m_query += tables.at(leftTableIndex) + '.'
					           + columns.at(indexColumn) + '='
					           + tables.at(i) + '.'
					           + columns.at(indexColumn);
					if (i != tables.size() - 1)
						m_query.append(" AND ");
				}
			}
			return m_query;
		}
		
		const string_t&
		QueryBuilder::selectQuery(const string_t& table, const string_t& column, string_t clause)
		{
			m_query.clear();
			m_query += "SELECT ";
			
			m_query += column;
			m_query += " FROM " + table;
			
			if (!clause.empty())
			{
				m_query += " WHERE ";
				m_query += clause;
			}
			
			return m_query;
		}
		
		const string_t&
		QueryBuilder::selectQuery(const string_t& table, const std::vector<string_t>& columns, ui32 indexColumn)
		{
			m_query.clear();
			m_query += "SELECT ";
			
			for (ui32 index = 0; index < columns.size(); index++)
			{
				if (index != indexColumn)
					m_query.append(columns.at(index));
				if (index != columns.size() - 1 && index != indexColumn)
					m_query.push_back(',');
				m_query.push_back(' ');
			}
			m_query += " FROM " + table;
			
			return m_query;
		}
		
		const string_t&
		QueryBuilder::selectQuery(const string_t& table, const std::vector<string_t>& columns, string_t clause)
		{
			m_query.clear();
			m_query += "SELECT ";
			
			for (const auto& column : columns)
			{
				m_query += column + ',';
			}
			m_query.pop_back();
			m_query.push_back(' ');
			
			m_query += " FROM " + table + ' ';
			
			m_query += clause;
			return m_query;
		}
		
		const string_t&
		QueryBuilder::selectLikeQuery(const std::vector<string_t>& tables, const std::vector<string_t>& columns,
		                              const string_t& text, ui32 comparatorColumn, ui32 leftTableIndex, ui32 fieldIndex,
		                              bool asRegex)
		{
			string_t __searchText {};
			if (asRegex)
				__searchText += '%';
			__searchText += text + '%';
			selectQuery(tables, columns, comparatorColumn, leftTableIndex);
			m_query += " WHERE " + columns.at(fieldIndex) + " LIKE \"" + __searchText.c_str() + "\"";
			return m_query;
		}
		
		const string_t&
		QueryBuilder::updateQuery(const string_t& tableName, const std::list<Value*>& dbValueList)
		{
			string_t params {};
			m_query.clear();
			
			for (const auto& value : dbValueList)
			{
				m_query += "UPDATE " + tableName + " SET ";
				m_query += value->getValueName() + "=, ";
			}
			return m_query;
		}
	}
}