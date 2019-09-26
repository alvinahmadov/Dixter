/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include <mutex>

#include "Macros.hpp"
#include "NodeEntry.hpp"
#include "Exception.hpp"

namespace Dixter
{
	// TNodeEntry implementation
	TNodeEntry::TNodeEntry(TNodeEntry::EException throws) noexcept
			: m_index(),
			  m_throw(throws),
			  m_nodeEntries(TEntryMap {})
	{ }
	
	TNodeEntry::~TNodeEntry() noexcept
	{ }
	
	bool TNodeEntry::insertEntry(TNodeData* nodeEntry)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		if (m_nodeEntries.find(m_index) == m_nodeEntries.end())
		{
			m_nodeEntries.insert({ m_index, std::shared_ptr<TNodeData>(nodeEntry) });
			++m_index;
			return true;
		}
		return false;
	}
	
	bool TNodeEntry::setEntry(const TString& key, const TUString& value)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		bool __found { checkKey(key) };
		if (__found)
		{
			for (auto& __valuePair : m_nodeEntries)
				for (auto& __node : __valuePair.second->getNodes())
					if (__node->m_name.compare(key) == 0)
						__node->m_value = value;
		}
		else if (m_throw == EException::Throw)
			throw TNotFoundException(
					"%s:%d Node data for name \"%s\" not found.", __FILE__, __LINE__, key);
		
		return __found;
	}
	
	bool TNodeEntry::setEntry(TSize index, const TUString& value)
	{
		std::lock_guard<std::mutex> l(m_mutex);
		if (checkIndex(index))
		{
			auto iter = m_nodeEntries.find(index);
			auto& __nodes = iter->second->getNodes();
			const auto& __nodeName = findEntry(value)->m_name;
			
			auto __f_pred = [ &__nodeName ](std::shared_ptr<TNode>& node)
			{ return node->m_name.compare(__nodeName) == 0; };
			
			std::replace_if(
					__nodes.begin(), __nodes.end(),
					__f_pred, dxMAKE_SHARED(TNode, __nodeName, value));
			return true;
		}
		return false;
	}
	
	bool TNodeEntry::removeEntry()
	{
		return false;
	}
	
	std::shared_ptr<const TNode>
	TNodeEntry::findEntry(const TString& key) const
	{
		std::lock_guard<std::mutex> l(m_mutex);
		std::shared_ptr<TNode> __data {};
		for (const auto& __valuePair : m_nodeEntries)
			for (std::shared_ptr<TNode>& __node : __valuePair.second->getNodes())
				if (__node->m_name.compare(key) == 0)
					__data = __node;
		
		if (m_throw == EException::Throw)
			if (not __data)
				throw TNotFoundException(
						"%s:%d Node data for \"%s\" not found.", __FILE__, __LINE__, key);
		
		return __data;
	}
	
	std::shared_ptr<const TNode>
	TNodeEntry::findEntry(const TUString& value) const
	{
		std::lock_guard<std::mutex> __lg(m_mutex);
		std::shared_ptr<TNode> __data;
		for (const auto& __valuePair : m_nodeEntries)
		{
			for (auto& __node : __valuePair.second->getNodes())
				if (__node->m_value.compare(value) == 0)
					__data = __node;
		}
		if (m_throw == EException::Throw)
			if (not __data)
				throw TNotFoundException(
						"%s:%d Node data for \"%s\" not found.", __FILE__, __LINE__, value.asUTF8());
		
		return __data;
	}
	
	std::shared_ptr<const TNodeData>
	TNodeEntry::findEntryData(Int32 index) const
	{
		std::lock_guard<std::mutex> __lg(m_mutex);
		std::shared_ptr<const TNodeData> __data;
		if (index > 0)
		{
			if (checkIndex(static_cast<TSize>(index)))
			{
				for (const auto& __p : m_nodeEntries)
				{
					if (__p.first == index)
						__data = __p.second;
				}
			}
		}
		
		if (m_throw == EException::Throw)
			if (not __data)
				throw TNotFoundException("%s:%d Node data for index \'%d\' not found.", __FILE__, __LINE__, index);
		
		return __data;
	}
	
	Int32 TNodeEntry::findEntryIndex(const TString& key) const
	{
		Int32 __index { -1 };
		for (const auto& __p : m_nodeEntries)
			for (const auto& __node : __p.second->getNodes())
				if (not __node->m_name.compare(key))
					__index = __p.first;
		
		return __index;
	}
	
	bool TNodeEntry::checkKey(const TString& key) const
	{
		bool __found(false);
		if (not key.empty())
		{
			try
			{
				if (findEntry(key) != nullptr)
					__found = true;
			}
			catch (std::exception& e)
			{
				__found = false;
				printerr(e.what())
			}
		}
		else
			__found = false;
		
		return __found;
	}
	
	bool TNodeEntry::checkIndex(TSize index) const
	{
		if (index >= std::numeric_limits<TSize>::min() and index <= std::numeric_limits<TSize>::max())
		{
			if (index <= m_nodeEntries.size())
			{
				if (m_nodeEntries.find(index) != m_nodeEntries.end())
					return true;
			}
		}
		return false;
	}
} // namespace Dixter