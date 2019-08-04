/**
 *  Copyright (C) 2013-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once


#include <unordered_map>

#include "Commons.hpp"

namespace Dixter
{
	template<typename ID = id_type>
	struct ElementProperty
	{
		ID id;
		string_t description;
		bool bAutoDelete;
	};
	
	template<typename ID>
	inline void setProperty(ElementProperty<ID>* properties, ID id, const string_t& description, bool autoDelete)
	{
		if (!properties)
		{
			properties = new ElementProperty<ID>;
		}
		properties->id = id;
		properties->description.clear();
		properties->description = description;
		properties->bAutoDelete = autoDelete;
	}
	
	template<typename ID>
	inline void setProperty(ElementProperty<ID>* properties, ID id, bool autoDelete)
	{
		if (!properties)
		{
			properties = new ElementProperty<ID>;
		}
		properties->id = id;
		properties->description.clear();
		properties->description = string_t();
		properties->bAutoDelete = autoDelete;
	}
	
	template<typename ID>
	inline void setNullProperty(ElementProperty<ID>* properties)
	{
		if (properties)
		{
			properties->id = static_cast<ID>(0);
			if (!properties->description.empty())
			{
				properties->description.clear();
			}
			SAFE_RELEASE(properties)
		}
	}
	
	template<class Element, typename ID = ui32>
	class GroupElement : public ComparableInterface<GroupElement<Element, ID>>
	{
	public:
		GroupElement();
		
		explicit GroupElement(Element& element, const ID& id, const string_t& description, bool autoDelete = true);
		
		explicit GroupElement(Element& element, const ID& id, bool autoRelease = true);
		
		~GroupElement();
		
		int compareTo(const GroupElement& other);
		
		bool equal(GroupElement& other) const;
		
		bool equal(const ID& id) const;
		
		bool empty() const;
		
		Element& getElement();
		
		ElementProperty<ID>* getProperties();
	
	private:
		Element m_element;
		
		ElementProperty<ID>* m_pProperties;
	};
	
	template<class Element, typename ID>
	void releaseElement(GroupElement<Element, ID>* element);
	
	template<typename Element, typename ID>
	struct ElementComparator
	{
		bool operator()(const GroupElement<Element, ID>& lhs, const GroupElement<Element, ID>& rhs)
		{
			return lhs.id == rhs.id;
		}
	};
}

namespace Dixter
{
	// Implementation
	template<class Element, typename ID>
	GroupElement<Element, ID>::GroupElement()
	{
		m_element = nullptr;
		setNullProperty(m_pProperties);
	}
	
	template<class Element, typename ID>
	GroupElement<Element, ID>::GroupElement(Element& element, const ID& id, const string_t& description, bool autoDelete)
			: m_pProperties { new ElementProperty<ID>() }
	{
		m_element = element;
		setProperty(m_pProperties, id, description, autoDelete);
	}
	
	template<class Element, typename ID>
	GroupElement<Element, ID>::GroupElement(Element& element, const ID& id, bool autoRelease)
			: m_pProperties { new ElementProperty<ID>() }
	{
		m_element = element;
		setProperty(m_pProperties, id, autoRelease);
	}
	
	template<class Element, typename ID>
	GroupElement<Element, ID>::~GroupElement()
	{
		releaseElement(this);
	}
	
	template<typename Element, typename ID>
	int GroupElement<Element, ID>::compareTo(const GroupElement<Element, ID>& other)
	{
		return static_cast<int>(m_pProperties->id) - static_cast<int>(other.m_pProperties->id);
	}
	
	template<class Element, typename ID>
	inline bool GroupElement<Element, ID>::equal(GroupElement<Element, ID>& other) const
	{
		return ElementComparator<Element, ID>(this, other);
	}
	
	template<class Element,
	         typename ID>
	inline bool GroupElement<Element, ID>::equal(const ID& id) const
	{
		return m_pProperties->id == id;
	}
	
	template<class Element, typename ID>
	inline bool GroupElement<Element, ID>::empty() const
	{
		return m_element == nullptr;
	}
	
	template<class Element, typename ID>
	Element& GroupElement<Element, ID>::getElement()
	{
		return m_element;
	}
	
	template<class Element, typename ID>
	ElementProperty<ID>*
	GroupElement<Element, ID>::getProperties()
	{
		return m_pProperties;
	}
	
	template<class Element, typename ID>
	inline void releaseElement(GroupElement<Element, ID>* element)
	{
		if (element->getProperties()->bAutoDelete)
		{
			SAFE_RELEASE(element->getElement())
		}
		setNullProperty(element->getProperties());
	}
}