/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "Exception.hpp"

namespace Dixter
{
	/// Group implementation
	template<class T, typename ID>
	TGroup<T, ID>::TGroup() noexcept
			: m_count(),
			  m_group(new TMultimap)
	{ }
	
	template<class T, typename ID>
	TGroup<T, ID>::TGroup(TGroup<T, ID>&& src) noexcept
			: m_group(std::move(src.m_group)),
			  m_count(std::move(src.m_count))
	{ }
	
	template<class T, typename ID>
	TGroup<T, ID>&
	TGroup<T, ID>::operator=(TGroup&& src) noexcept
	{
		m_group = std::move(src.m_group);
		m_count = std::move(src.m_count);
	}
	
	template<class T, typename ID>
	TGroup<T, ID>::~TGroup() noexcept
	{
		for (auto& group_pair : *m_group)
			for (auto& __element : *group_pair.second)
				delete __element;
		
		delete m_group;
	}
	
	template<class T, typename ID>
	typename TGroup<T, ID>::TObject*
	TGroup<T, ID>::doAdd(const TGroup<T, ID>::TKey& groupName,
						 TGroup<T, ID>::TElement* groupElement)
	{
		std::lock_guard<std::mutex> addLock(m_mutex);
		bool __groupExists(this->hasGroup(groupName));
		bool __elementExists(this->hasElement(groupName, groupElement->getProperties()->getId()));
		
		// create both group and element
		if (not __groupExists and not __elementExists)
		{
			/// If we don't have the item then create group and add child item to the new group
			m_group->insert({ groupName, new TGroup<T, ID>::TContainer { groupElement }});
			++m_count;
		}
		else if (__groupExists and not __elementExists)
		{
			// create only item in existing group
			this->getGroup(groupName)->push_back(groupElement);
		}
		else
			throw TIllegalArgumentException(
					"%s:%d\nThe specified element can not be added to the group %s",
					__FILE__, __LINE__, groupName);
		
		return groupElement->getElement();
	}
	
	// Mutators
	
	template<class T, typename ID>
	template<class TCast>
	TCast*
	TGroup<T, ID>::add(const TGroup<T, ID>::TKey& groupName, TObject* element, ID id,
					   bool autoRelease, TStringView description)
	{
		if (this->hasElement(groupName, id))
			return this->get<TCast>(groupName, id);
		
		dxASSERT_MSG(this->isValidGroupName(groupName), "Not a valid group m_name.");
		dxASSERT_MSG(this->isValidElement(element), "Not a valid element.");
		try
		{
			return dynamic_cast<TCast*>(this->doAdd(
					groupName,
					new TGroupElement<TObject*, ID> { element, id, description, autoRelease }));
		}
		catch (TException& e)
		{
			printerr(e.getMessage())
		}
		return nullptr;
	}
	
	template<class T, typename ID>
	template<class TCast>
	TCast*
	TGroup<T, ID>::add(TObject* element, ID id, bool autoRelease,
					   TStringView description)
	{
		dxASSERT_MSG(this->isValidElement(element), "Not a valid element.");
		try
		{
			return dynamic_cast<TCast*>(this->doAdd(
					s_defaultGroup,
					new TGroupElement<TObject*, ID>(element, id, description, autoRelease)));
		}
		catch (TException& e)
		{
			printerr(e.what())
		}
		return nullptr;
	}
	
	template<class T, typename ID>
	bool TGroup<T, ID>::remove(const TGroup<T, ID>::TKey& groupName, ID id)
	{
		bool __bExists(this->hasElement(groupName, id));
		TGroup<T, ID>::TMapped __elementList {};
		if (__bExists)
		{
			std::for_each(
					m_group->begin(), m_group->end(),
					[ & ](TGroup<T, ID>::TValue& values)
					{
						if (values.first == groupName)
						{
							__elementList = values.second;
							for (auto __listIter = __elementList->begin();
								 __listIter != __elementList->end();
								 ++__listIter)
							{
								if ((( *__listIter )->equal(id)) and not(( *__listIter )->empty()))
								{
									this->releaseElement(( *__listIter ));
									__elementList->erase(__listIter);
									--m_count;
									__bExists = this->hasElement(groupName, id);
									return not __bExists;
								}
							}
						}
						return __bExists;
					});
		}
		return false;
	}
	
	template<class T, typename ID>
	inline void TGroup<T, ID>::clear()
	{
		m_group->clear();
	}
	
	template<class T, typename ID>
	TGroup<T, ID>&
	TGroup<T, ID>::append(T* element, ID id)
	{
		auto mp = this->getGroup(getSize() - 1);
		
		mp->insert(mp->end(), new TGroupElement<TObject*, ID>(element, id));
		return *this;
	}
	
	template<class T, typename ID>
	inline typename TGroup<T, ID>::TMapped&
	TGroup<T, ID>::getGroup(const TGroup<T, ID>::TKey& groupName)
	{
		if (this->hasGroup(groupName))
			return m_group->at(groupName);
		else
			throw TNotFoundException(
					"%s:%d\nThe group %s is not found",
					__FILE__, __LINE__, groupName);
	}
	
	template<class T, typename ID>
	template<class TCast>
	TCast*
	TGroup<T, ID>::get(ID id)
	{
		auto __first = m_group->begin();
		auto __last = m_group->end();
		
		for (auto __iter = __first;
			 __iter != __last; __iter++)
		{
			auto __container = __iter->second;
			for (auto __groupItem : *__container)
				if (__groupItem->equal(id))
					return dynamic_cast<TCast*>(__groupItem->getElement());
		}
		throw TNotFoundException("%s:%d Item with id %d not found.", __FILE__, __LINE__, id);
	}
	
	template<class T, typename ID>
	template<class TCast>
	inline TCast*
	TGroup<T, ID>::get(const TGroup<T, ID>::TKey& groupName, ID id)
	{
		if (m_group->find(groupName) == m_group->end())
			throw TNotFoundException(
					"%s:%d Item with id %d not found in group \"%s\"",
					__FILE__, __LINE__, groupName);
		
		for (const auto& __groupItem : *m_group->find(groupName)->second)
		{
			if (__groupItem->equal(id))
				return dynamic_cast<TCast*>(__groupItem->getElement());
		}
		throw TNotFoundException(
				"%s:%d Item with id %d not found in group \"%s\"",
				__FILE__, __LINE__, groupName);
	}
	
	template<class T, typename ID>
	template<
			typename TReturn,
			typename... TArgs
	>
	void
	TGroup<T, ID>::forEach(TReturn(T::*method)(TArgs ...), TArgs... args)
	{
		auto __methodCallback = TMethodCallback<T, TReturn, TArgs...>(method);
		std::for_each(
				begin(), end(), [ &__methodCallback, &args... ](auto& pair)
				{
					for (TElement* __item : *pair.second)
					{
						if (not __item->empty())
							__methodCallback(__item->getElement(), std::forward<TArgs>(args)...);
					}
				});
	}
	
	template<class T, typename ID>
	template<
			typename TReturn,
			typename... TArgs
	>
	inline void
	TGroup<T, ID>::forEach(const TGroup<T, ID>::TKey& groupName,
						   TReturn(T::*method)(TArgs...),
						   TArgs... args)
	{
		auto __methodCallback = TMethodCallback<T, TReturn, TArgs...>(method);
		for (auto& __item : *getGroup(groupName))
			__methodCallback(__item->getElement(), std::forward<TArgs>(args)...);
	}
	
	// Accessors
	template<class T, typename ID>
	inline TSize
	TGroup<T, ID>::getSize() const
	{
		return m_group->size();
	}
	
	template<class T, typename ID>
	inline TSize
	TGroup<T, ID>::getItemCount(const TGroup<T, ID>::TKey& groupName) const
	{
		auto __groupItems = this->getGroup(groupName);
		if (__groupItems->size())
			return this->getGroup(groupName)->size();
		return 0;
	}
	
	template<class T, typename ID>
	inline bool TGroup<T, ID>::empty() const noexcept
	{
		return m_group->empty();
	}
	
	template<class T, typename ID>
	inline bool TGroup<T, ID>::empty(const TGroup<T, ID>::TKey& key) const noexcept
	{
		this->getGroup(key)->empty();
	}
	
	template<class T, typename ID>
	inline bool TGroup<T, ID>::isEmpty() const noexcept
	{
		return this->empty();
	}
	
	template<class T, typename ID>
	inline bool TGroup<T, ID>::isEmpty(const TGroup<T, ID>::TKey& key) const noexcept
	{
		return this->empty(key);;
	}
	
	template<class T, typename ID>
	typename TGroup<T, ID>::TIterator
	inline TGroup<T, ID>::begin() noexcept
	{
		return m_group->begin();
	}
	
	template<class T, typename ID>
	typename TGroup<T, ID>::TIterator
	inline TGroup<T, ID>::end() noexcept
	{
		return m_group->end();
	}
	
	template<class T, typename ID>
	typename TGroup<T, ID>::TConstIterator
	inline TGroup<T, ID>::cbegin() const noexcept
	{
		return m_group->begin();
	}
	
	template<class T, typename ID>
	typename TGroup<T, ID>::TConstIterator
	inline TGroup<T, ID>::cend() const noexcept
	{
		return m_group->end();
	}
	
	template<class T, typename ID>
	bool
	TGroup<T, ID>::hasElement(const TGroup<T, ID>::TKey& groupName, const ID& id) const
	{
		bool __bExists {};
		
		if (this->hasGroup(groupName))
		{
			auto __first = m_group->begin();
			auto __last = m_group->end();
			
			for (auto __iterator = __first; __iterator != __last; ++__iterator)
			{
				for (const auto& __element : *__iterator->second)
				{
					if (__element->getProperties()->getId() == id and __element->getElement() != nullptr)
						__bExists = true;
				}
			}
		}
		return __bExists;
	}
	
	template<class T, typename ID>
	inline bool
	TGroup<T, ID>::isValidGroupName(const TGroup<T, ID>::TKey& groupName) const
	{
		return this->hasGroup(groupName) or ( groupName.size() > 0 );
	}
	
	template<class T, typename ID>
	inline bool
	TGroup<T, ID>::isValidElement(const TGroup<T, ID>::TObject* element) const
	{
		return ( element != nullptr );
	}
	
	template<class T, typename ID>
	bool TGroup<T, ID>::hasGroup(const TGroup<T, ID>::TKey& groupName) const
	{
		for (const TValue& __value : *m_group)
			if (__value.first.compare(groupName) == 0)
				return true;
		return false;
	}
	
	template<class T, typename ID>
	const typename TGroup<T, ID>::TKey
			TGroup<T, ID>::s_defaultGroup("Default");
}