/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include <memory>
#include <list>
#include "Types.hpp"

namespace Dixter
{
	/**
	 * Field of attributes in consfiguration file for specific nodes
	 **/
	struct Attribute
	{
		Attribute(const string_t& attributeName, const ustring_t& attributeValue);
		
		string_t name;
		ustring_t value;
		size_t index;
	};
	
	/**
	 * Stores data parsed from settings file as node key and node value
	 **/
	struct Node
	{
		/**
		 * Constructor saving node's key and value
		 * @param nodeName name of the node
		 * @param nodeValue value of node
		 * */
		Node(const string_t& nodeName, const ustring_t& nodeValue, const string_t& parent = "");
		
		bool equal(const Node& other) const;
		
		bool has(const string_t& name) const;
		
		string_t name;
		ustring_t value;
		string_t parent;
		
		size_t index;
	};
	
	using Nodes         = std::list<Node*>;
	using Attributes    = std::list<std::shared_ptr<Attribute>>;
	
	/**
	 * Parses information for configuration and saves it
	 **/
	class NodeData
	{
	public:
		/**
		 * Default constructor
		 **/
		NodeData();
		
		/**
		 * Default destructor
		 **/
		~NodeData();
		
		/**
		 * Add node for configuration
		 */
		void insertData(const string_t& nodeName, const ustring_t& nodeValue, const string_t& parent = "",
		                const string_t& attributeName = "", const ustring_t& attributeValue = ustring_t());
		
		/**
		 * \class NodeData
		 * \brief Tries to set value of the concrete node.
		 * \param name Name of the node to change value.
		 * \param value Value to set.
		 * \throws NotFoundException
		 * */
		void setValue(const string_t& name, const ustring_t& value);
		
		/**
		 * \class NodeData
		 * \brief Tries to get value of the concrete node.
		 * \param name Name of node containing value.
		 * \returns Value of node.
		 * \throws NotFoundException
		 * */
		ustring_t getValue(const string_t& name) const;
		
		template<typename Container_T>
		void getValues(const string_t& name, Container_T& values) const
		{
			values.push_back(getValue(name));
		}
		
		const Nodes& getNodes() const;
		
		Nodes& getNodes();
	
	private:
		/**
		 * The list for nodes
		 **/
		Nodes nodes;
		
		/**
		 * Attributes for every node
		 **/
		Attributes attributes;
	};
}