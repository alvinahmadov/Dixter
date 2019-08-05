/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#include "Node.hpp"
#include "Commons.hpp"

namespace Dixter
{
	//////////////////////////////
	// Attribute Implementation //
	//////////////////////////////
	Attribute::Attribute(const string_t& attributeName, const ustring_t& attributeValue)
			: name {attributeName},
			  value {attributeValue},
			  index {}
	{}
	
	///Node Implementation
	Node::Node(const string_t& nodeName, const ustring_t& nodeValue, const string_t& parent)
			: name {nodeName},
			  value {nodeValue},
			  parent {parent},
			  index {0}
	{}
	
	bool Node::equal(const Node& aNode) const
	{
		return ((name == aNode.name) && (value == aNode.value) && (index == aNode.index));
	}
	
	bool Node::has(const string_t& aName) const
	{
		return (this->name.compare(aName) == 0);
	}
	
	///NodeData Implementation
	NodeData::NodeData()
			: nodes {},
			  attributes {}
	{}
	
	void NodeData::insertData(const string_t& nodeName, const ustring_t& nodeValue, const string_t& parent,
	                          const string_t& attributeName, const ustring_t& attributeValue)
	{
		static string_t __parentBefore {parent};
		static string_t __startNode {nodeName};
		static size_t __localIndex {};
		
		if (parent != __parentBefore)
		{
			__startNode = nodeName;
			__parentBefore = parent;
		}
		
		nodes.push_back(std::make_shared<Node>(nodeName, nodeValue, parent));
		if (nodeName.compare(__startNode) == 0)
			__localIndex = 0;
		else
			__localIndex++;
		
		if (!attributeName.empty() && !attributeValue.empty())
		{
			attributes.push_back(std::make_shared<Attribute>(attributeName, attributeValue));
			attributes.back()->index = __localIndex;
		}
		nodes.back()->index = __localIndex;
	}
	
	void NodeData::setValue(const string_t& name, const ustring_t& value)
	{
		for (const auto& __node : getNodes())
			if (__node->has(name))
				__node->value = value;
		
		throw NotFoundException {"%s:%d No node with name %s.", __FILE__, __LINE__, name};
	}
	
	ustring_t
	NodeData::getValue(const string_t& name) const
	{
		for (const auto& __node : getNodes())
			if (__node->has(name))
				return __node->value;
		
		throw NotFoundException {"%s:%d No node with name %s.", __FILE__, __LINE__, name};
	}
	
	const Nodes&
	NodeData::getNodes() const
	{
		return nodes;
	}
	
	Nodes&
	NodeData::getNodes()
	{
		return nodes;
	}
}