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
	class ElementProperty
	{
	public:
		explicit ElementProperty(ID id, const string_t& description, bool autoDelete) dxDECL_NOEXCEPT
		{
			m_id = id;
			m_description.clear();
			m_description = description;
			m_autoDelete = autoDelete;
		}
		
		explicit ElementProperty(ID id, bool autoDelete) dxDECL_NOEXCEPT
		{
			m_id = id;
			m_description.clear();
			m_description = string_t();
			m_autoDelete = autoDelete;
		}
		
		inline void reset()
		{
			this->m_id = static_cast<ID>(0);
			if (!m_description.empty())
			{
				m_description.clear();
			}
		}
		
		ID getId() const
		{
			return m_id;
		}
		
		inline bool isAutoDeletable() const
		{
			return m_autoDelete;
		}
		
		inline const string_t& getDescription() const
		{
			return m_description;
		}
	
	private:
		ID m_id;
		
		bool m_autoDelete;
		
		string_t m_description;
	};
	template<class Element, typename ID = ui32>
	class GroupElement : public ComparableInterface<GroupElement<Element, ID>>, public DefaultNonCopyable
	{
	public:
		GroupElement() = default;
		
		explicit GroupElement(Element& element, const ID& id, const string_t& description, bool autoDelete = true);
		
		explicit GroupElement(Element& element, const ID& id, bool autoRelease = true);
		
		~GroupElement();
		
		int compareTo(const GroupElement& other);
		
		bool equal(GroupElement& other) const;
		
		bool equal(const ID& id) const;
		
		bool empty() const;
		
		const Element& getElement() const;
		
		Element& getElement();
		
		const ElementProperty<ID>* getProperties() const;
	
	private:
		Element m_element;
		
		ElementProperty<ID>* m_pProperties;
	};
	template<typename Element, typename ID>
	struct ElementComparator
	{
		constexpr bool operator()(const GroupElement<Element, ID>& lhs,
		                          const GroupElement<Element, ID>& rhs) const
		{
			return lhs.getProperties()->getId() == rhs.getProperties()->getId();
		}
	};
}
namespace Dixter
{
	// Implementation
	template<class Element, typename ID>
	GroupElement<Element, ID>::GroupElement(Element& element, const ID& id, const string_t& description, bool autoDelete)
			: m_pProperties { new ElementProperty<ID>(id, description, autoDelete) }
	{
		m_element = element;
	}
	
	template<class Element, typename ID>
	GroupElement<Element, ID>::GroupElement(Element& element, const ID& id, bool autoRelease)
			: m_pProperties { new ElementProperty<ID>(id, autoRelease) }
	{
		m_element = element;
	}
	
	template<class Element, typename ID>
	GroupElement<Element, ID>::~GroupElement()
	{
		releaseElement(this);
	}
	
	template<typename Element, typename ID>
	int GroupElement<Element, ID>::compareTo(const GroupElement<Element, ID>& other)
	{
		return static_cast<int>(m_pProperties->getId()) - static_cast<int>(other.m_pProperties->getId());
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
		return m_pProperties->getId() == id;
	}
	
	template<class Element, typename ID>
	inline bool GroupElement<Element, ID>::empty() const
	{
		return m_element == nullptr;
	}
	
	template<class Element, typename ID>
	const Element& GroupElement<Element, ID>::getElement() const
	{
		return m_element;
	}
	
	template<class Element, typename ID>
	Element& GroupElement<Element, ID>::getElement()
	{
		return m_element;
	}
	
	template<class Element, typename ID>
	const ElementProperty<ID>*
	GroupElement<Element, ID>::getProperties() const
	{
		return m_pProperties;
	}
	
	template<class Element, typename ID>
	inline void releaseElement(GroupElement<Element, ID>* element)
	{
		if (element->getProperties()->isAutoDeletable())
		{
			SAFE_RELEASE(element->getElement())
		}
	}
}