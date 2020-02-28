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
#include <deque>
#include "Types.hpp"

namespace Dixter
{
	/**
	 * Field of attributes in consfiguration file for specific nodes
	 **/
	struct TAttribute
	{
		TAttribute() = delete;
		
		explicit TAttribute(const TString& attributeName,
							const TUString& attributeValue) noexcept;
		
		TAttribute(const TAttribute&) = default;
		
		TAttribute& operator=(const TAttribute&) = default;
		
		TSize m_index;
		
		TString m_name;
		
		TUString m_value;
	};
	
	/**
	 * Stores data parsed from settings file as node key and node value
	 **/
	struct TNode
	{
		TNode() = delete;
		
		/**
		 * Constructor saving node's key and value
		 * @param nodeName name of the node
		 * @param nodeValue value of node
		 * */
		explicit TNode(const TString& nodeName, const TUString& nodeValue,
					   const TString& parent = TString()) noexcept;
		
		TNode(const TNode&) = default;
		
		TNode& operator=(const TNode&) = default;
		
		bool equal(const TNode& node) const;
		
		bool has(const TString& name) const;
		
		TSize m_index;
		
		TString m_name;
		
		TUString m_value;
		
		TString m_parent;
	};
	
	/**
	 * Parses information for configuration and saves it
	 **/
	class TNodeData final
	{
	public:
		using TNodeList         = std::deque<std::shared_ptr<TNode>>;
		using TAttributeList    = std::deque<std::shared_ptr<TAttribute>>;
	public:
		/**
		 * Default constructor
		 **/
		TNodeData() noexcept;
		
		~TNodeData() noexcept = default;
		
		/**
		 * Add node for configuration
		 */
		void insertData(const TString& nodeName, const TUString& nodeValue,
						const TString& parent = TString(),
						const TString& attributeName = TString(),
						const TUString& attributeValue = TString());
		
		/**
		 * \class NodeData
		 * \brief Tries to set value of the concrete node.
		 * \param name Name of the node to change value.
		 * \param value Value to set.
		 * \throws NotFoundException
		 * */
		void setValue(const TString& name, const TUString& value);
		
		/**
		 * \class NodeData
		 * \brief Tries to get value of the concrete node.
		 * \param name Name of node containing value.
		 * \returns Value of node.
		 * \throws NotFoundException
		 * */
		TUString getValue(const TString& name) const;
		
		template<typename TContainer>
		void getValues(const TString& name, TContainer& values) const
		{
			values.push_back(getValue(name));
		}
		
		const TNodeList& getNodes() const;
		
		TNodeList& getNodes();
	
	private:
		/**
		 * The list for nodes
		 **/
		TNodeList m_nodes;
		
		/**
		 * Attributes for every node
		 **/
		TAttributeList m_attributes;
	};
} // namespace Dixter