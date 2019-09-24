/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "NodeData.hpp"
#include "Exception.hpp"

namespace Dixter
{
	//////////////////////////////
	// TAttribute Implementation //
	//////////////////////////////
	TAttribute::TAttribute(const TString& attributeName, const TUString& attributeValue) noexcept
			: m_index(),
			  m_name(attributeName),
			  m_value(attributeValue)
	{ }
	
	///Node Implementation
	TNode::TNode(const TString& nodeName, const TUString& nodeValue, const TString& parent) noexcept
			: m_index(),
			  m_name(nodeName),
			  m_value(nodeValue),
			  m_parent(parent)
	{ }
	
	bool TNode::equal(const TNode& node) const
	{
		return (( m_name.compare(node.m_name) == 0) and
				( m_value.compare(node.m_value) == 0 ) and
				( m_index == node.m_index ));
	}
	
	bool TNode::has(const TString& name) const
	{
		return not m_name.compare(name);
	}
	
	///NodeData Implementation
	void TNodeData::insertData(const TString& nodeName, const TUString& nodeValue,
							   const TString& parent,
							   const TString& attributeName, const TUString& attributeValue)
	{
		static TString __parentBefore(parent);
		static TString __startNode(nodeName);
		static TSize __index {};
		
		if (parent != __parentBefore)
		{
			__startNode = nodeName;
			__parentBefore = parent;
		}
		
		m_nodes.push_back(dxMAKE_SHARED(TNode, nodeName, nodeValue, parent));
		if (not nodeName.compare(__startNode))
			__index = 0;
		else
			__index++;
		
		if (not attributeName.empty() and not attributeValue.empty())
		{
			m_attributes.push_back(dxMAKE_SHARED(TAttribute, attributeName, attributeValue));
			m_attributes.back()->m_index = __index;
		}
		m_nodes.back()->m_index = __index;
	}
	
	void TNodeData::setValue(const TString& name, const TUString& value)
	{
		for (const auto& __node : getNodes())
			if (__node->has(name))
				__node->m_value = value;
		
		throw TNotFoundException("%s:%d No node with name %s.", __FILE__, __LINE__, name);
	}
	
	TUString
	TNodeData::getValue(const TString& name) const
	{
		for (const auto& __node : getNodes())
			if (__node->has(name))
				return __node->m_value;
		
		throw TNotFoundException("%s:%d No node with name %s.", __FILE__, __LINE__, name);
	}
	
	const TNodeData::TNodeList&
	TNodeData::getNodes() const
	{
		return m_nodes;
	}
	
	TNodeData::TNodeList&
	TNodeData::getNodes()
	{
		return m_nodes;
	}
} // namespace Dixter