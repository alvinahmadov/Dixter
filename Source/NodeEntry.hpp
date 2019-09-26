/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <map>
#include <mutex>
#include <algorithm>

#include "Types.hpp"
#include "NodeData.hpp"
#include "MethodCallback.hpp"

namespace Dixter
{
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 * \brief Iterface class to store data configuration data.
	 *
	 * Stores data read from configuration reader/writer in an
	 * indexed map, to simplify data access.
	 * */
	class TNodeEntry final
	{
	public:
		using TEntryMap = std::map<Int32, std::shared_ptr<TNodeData>>;
		using TIterator = TEntryMap::iterator;
		using TConstIterator = TEntryMap::const_iterator;
	public:
		/**
		 * \namespace Dixter
		 * \brief Trigger for exception catching for NodeEntry class
		 */
		enum class EException
		{
			Throw,
			DontThrow
		};
	public:
		/**
		 * \class NodeEntry
		 * \brief ctor.
		 * \param throw_ If data to return is null or empty,
		 * class methods will throw exceptions.
		 * */
		explicit TNodeEntry(EException throw_ = EException::Throw) noexcept;
		
		~TNodeEntry() noexcept;
		
		/**
		 * \class NodeEntry
		 * \brief Inserts data entry.
		 * \param nodeEntry  Entry to insert.
		 * \returns true if data was inserted, false otherwise.
		 * */
		bool insertEntry(TNodeData* nodeEntry);
		
		bool setEntry(const TString& key, const TUString& value);
		
		bool setEntry(TSize index, const TUString& value);
		
		/**
		 * \class NodeEntry
		 * \brief Removes data entry.
		 * \returns true if data was removed, false otherwise.
		 * */
		bool removeEntry();
		
		/**
		 * \class NodeEntry
		 * \brief Find matching entry by node name.
		 * \param key Node name as a key to find corresponding node.
		 * \returns Node.
		 *
		 * Depending on \c m_throw value throws exception if node
		 * haven't found else returns null.
		 * */
		std::shared_ptr<const TNode>
		findEntry(const TString& key) const;
		
		/**
		 * \class NodeEntry
		 * \brief Find matching entry by node value.
		 * \param value Node value as a key to find corresponding
		 * node.
		 * \returns Node.
		 *
		 * Depending on \c m_throw value throws exception if node
		 * haven't found else returns null.
		 * */
		std::shared_ptr<const TNode>
		findEntry(const TUString& value) const;
		
		/**
		 * \class NodeEntry
		 * \brief Find node data by index.
		 * \param index Index of data.
		 * \returns Node Data.
		 *
		 * Depending on \c m_throw value throws exception if
		 * node data haven't found else returns null.
		 * */
		std::shared_ptr<const TNodeData>
		findEntryData(Int32 index) const;
		
		Int32 findEntryIndex(const TString& key) const;
		
		/**
		 * \class NodeEntry
		 * \brief Get number of data stored in NodeEntry.
		 * \returns Size of stored data.
		 * */
		TSize getSize() const;
		
		/**
		 * \class NodeEntry
		 * \brief Calls NodeData methods on stored data
		 * with arguments.
		 * \tparam TReturn Return type of NodeData method.
		 * \tparam TArgs Argument types for NodeData method.
		 * \param method NodeData method to call.
		 * \param args Arguments of NodeData method.
		 * */
		template<
				typename TReturn,
				typename... TArgs
		>
		void forEach(TReturn(TNodeData::*method)(TArgs...), TArgs... args);
		
		TIterator begin();
		
		TIterator end();
		
		TConstIterator cbegin();
		
		TConstIterator cend();
	
	private:
		bool checkKey(const TString& key) const;
		
		bool checkIndex(TSize index) const;
	
	private:
		Int32 m_index;
		EException m_throw;
		TEntryMap m_nodeEntries;
		mutable std::mutex m_mutex;
	};
}

namespace Dixter
{
	inline TSize TNodeEntry::getSize() const
	{
		return m_nodeEntries.size();
	}
	
	inline TNodeEntry::TIterator
	TNodeEntry::begin()
	{
		return m_nodeEntries.begin();
	}
	
	inline TNodeEntry::TIterator
	TNodeEntry::end()
	{
		return m_nodeEntries.end();
	}
	
	inline TNodeEntry::TConstIterator
	TNodeEntry::cbegin()
	{
		return m_nodeEntries.cbegin();
	}
	
	inline TNodeEntry::TConstIterator
	TNodeEntry::cend()
	{
		return m_nodeEntries.cend();
	}
	
	template<typename TReturn, typename... TArgs>
	void TNodeEntry::forEach(TReturn(TNodeData::*method)(TArgs... args),
							 TArgs... args)
	{
		auto __methodCallback = TMethodCallback<TNodeData, TReturn, TArgs...>(method);
		std::for_each(
				m_nodeEntries.begin(), m_nodeEntries.end(),
				[ &__methodCallback, &args... ](TNodeEntry::TEntryMap::value_type& pair)
				{
					if (pair.second)
						__methodCallback(pair.second, args...);
				});
	}
} // namespace Dixter