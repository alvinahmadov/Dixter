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
	template<typename ID = TId>
	class TElementProperty
	{
	public:
		#ifdef HAVE_CXX17
		using TDescription = TStringView;
		#else
		using TDescription = const TString&;
		#endif
	public:
		TElementProperty(ID id, TDescription description, bool autoDelete) noexcept
		{
			m_id = id;
			m_description.clear();
			m_description = description.data();
			m_autoDelete = autoDelete;
		}
		
		explicit TElementProperty(ID id, bool autoDelete) noexcept
		{
			m_id = id;
			m_description.clear();
			m_description = TString();
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
		
		inline TDescription getDescription() const
		{
			return m_description;
		}
	
	private:
		ID m_id;
		
		bool m_autoDelete;
		
		TString m_description;
	};
	template<class Element, typename ID = UInt32>
	class TGroupElement : public IComparable<TGroupElement<Element, ID>>,
						  public TNonCopyable
	{
	public:
		using TDescription = typename TElementProperty<ID>::TDescription;
	public:
		TGroupElement() = default;
		
		TGroupElement(Element& element, const ID& id, TDescription description, bool autoDelete = true);
		
		TGroupElement(Element& element, const ID& id, bool autoRelease = true);
		
		~TGroupElement() noexcept;
		
		int compareTo(const TGroupElement& other) noexcept;
		
		bool equal(TGroupElement& other) const;
		
		bool equal(const ID& id) const;
		
		bool empty() const;
		
		const Element& getElement() const;
		
		Element& getElement();
		
		const TElementProperty<ID>* getProperties() const;
		
		void release() noexcept;
	
	private:
		Element m_element;
		
		TElementProperty<ID>* m_pProperties;
	};
	template<typename Element, typename ID>
	struct ElementComparator
	{
		constexpr bool operator()(const TGroupElement<Element, ID>& lhs,
								  const TGroupElement<Element, ID>& rhs) const
		{
			return lhs.getProperties()->getId() == rhs.getProperties()->getId();
		}
	};
} // namespace Dixter


namespace Dixter
{
	// Implementation
	template<class Element, typename ID>
	TGroupElement<Element, ID>::
	TGroupElement(Element& element, const ID& id,
				  TGroupElement<Element, ID>::TDescription description, bool autoDelete)
			: m_pProperties(new TElementProperty<ID>(id, description, autoDelete))
	{
		m_element = element;
	}
	
	template<class Element, typename ID>
	TGroupElement<Element, ID>::TGroupElement(Element& element, const ID& id, bool autoRelease)
			: m_pProperties(new TElementProperty<ID>(id, autoRelease))
	{
		m_element = element;
	}
	
	template<class Element, typename ID>
	TGroupElement<Element, ID>::~TGroupElement() noexcept
	{
		release();
	}
	
	template<typename Element, typename ID>
	int
	TGroupElement<Element, ID>::compareTo(const TGroupElement<Element, ID>& other) noexcept
	{
		return static_cast<int>(m_pProperties->getId()) - static_cast<int>(other.m_pProperties->getId());
	}
	
	template<class Element, typename ID>
	inline bool TGroupElement<Element, ID>::equal(TGroupElement<Element, ID>& other) const
	{
		return ElementComparator<Element, ID>(this, other);
	}
	
	template<
			class Element,
			typename ID
	>
	inline bool TGroupElement<Element, ID>::equal(const ID& id) const
	{
		return m_pProperties->getId() == id;
	}
	
	template<class Element, typename ID>
	inline bool TGroupElement<Element, ID>::empty() const
	{
		return m_element == nullptr;
	}
	
	template<class Element, typename ID>
	const Element& TGroupElement<Element, ID>::getElement() const
	{
		return m_element;
	}
	
	template<class Element, typename ID>
	Element& TGroupElement<Element, ID>::getElement()
	{
		return m_element;
	}
	
	template<class Element, typename ID>
	const TElementProperty<ID>*
	TGroupElement<Element, ID>::getProperties() const
	{
		return m_pProperties;
	}
	
	template<class Element, typename ID>
	void TGroupElement<Element, ID>::release() noexcept
	{
		if (getProperties()->isAutoDeletable())
			delete m_element;
	}
} // namespace Dixter