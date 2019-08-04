/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

namespace Dixter
{
	/// Group implementation
	template<class T, typename ID>
	Group<T, ID>::Group()
			: m_count { 0 },
			  m_group { new Group<T, ID>::multimap_type() }
	{ }
	
	template<class T, typename ID>
	Group<T, ID>::Group(Group <T, ID>&& src)
	{
		if (this != std::addressof(src))
		{
			m_group = std::move(src.m_group);
			m_count = std::move(src.m_count);
		}
	}
	
	template<class T, typename ID>
	Group<T, ID>::~Group()
	{
		for (auto& group_pair : *m_group)    // pair<string, list<GE*>*>
			for (auto& __element : *group_pair.second)
			{
				SAFE_RELEASE(__element)
			}
		SAFE_RELEASE(m_group)
	}
	
	template<class T, typename ID>
	typename Group<T, ID>::object_type*
	Group<T, ID>::add(Group<T, ID>::key_type& groupName, Group<T, ID>::element_type* groupElement)
	{
		std::lock_guard<std::mutex> addLock(m_mutex);
		bool __bGroupExists { hasGroup(groupName) };
		bool __bElementExist { hasElement(groupName, groupElement->getProperties()->id) };
		
		// create both group and m_element
		if (!__bGroupExists && !__bElementExist)
		{
			/// If we don't have the item then create group and add child item to the new group
			m_group->insert(Group<T, ID>::value_type { groupName, (new Group<T, ID>::container_type { groupElement }) });
			m_count++;
		} else if (__bGroupExists && !__bElementExist)
		{
			// create only item in existing group
			getGroup(groupName)->push_back(groupElement);
		} else
		{
			throw IllegalArgumentException("%s:%d\nThe specified m_element can not be added to the group %s");
		}
		return groupElement->getElement();
	}
	
	template<class T, typename ID>
	template<class cast_type>
	inline cast_type*
	Group<T, ID>::add(Group<T, ID>::key_type& groupName, object_type* element, ID id,
	                  bool autoRelease, const string_t& description)
	{
		if (hasElement(groupName, id))
		{
			return get<cast_type>(groupName, id);
		}
		try
		{
			// assert(validGroupName(groupName));
			// assert(validElement(element));
			
			auto gElement = new GroupElement<object_type*, ID>(element, id, description, autoRelease);
			
			return dynamic_cast<cast_type*>(add(groupName, gElement));
		} catch (Exception& e)
		{
			printerr(e.what())
		}
		return nullptr;
	}
	
	template<class T, typename ID>
	template<class cast_type>
	inline cast_type*
	Group<T, ID>::add(object_type* element, ID id,
	                  bool autoRelease, const string_t& description)
	{
		key_type groupNameDefault {"Default"};
		
		try
		{
			assert(validElement(element));
			
			auto gElement = new GroupElement<object_type*, ID>(element, id, description, autoRelease);
			
			return dynamic_cast<cast_type*>(add(groupNameDefault, gElement));
		} catch (Exception& e)
		{
			printerr(e.what())
		}
		return nullptr;
	}
	
	template<class T, typename ID>
	bool Group<T, ID>::remove(Group<T, ID>::key_type& groupName, ID id)
	{
		bool __bExists { hasElement(groupName, id) };
		Group<T, ID>::mapped_type __elementList = nullptr;
		if (__bExists)
		{
			std::for_each(m_group->begin(), m_group->end(), [ & ](Group<T, ID>::value_type& values)
			{
				if (values.first == groupName)
				{
					__elementList = values.second;
					for (auto __listIter = __elementList->begin(); __listIter != __elementList->end(); __listIter++)
					{
						if (((*__listIter)->equal(id)) && !((*__listIter)->empty()))
						{
							releaseElement((*__listIter));
							__elementList->erase(__listIter);
							--m_count;
							__bExists = hasElement(groupName, id);
							return !__bExists;
						}
					}
				}
				return __bExists;
			});
		}
		return false;
	}
	
	template<class T, typename ID>
	Group<T, ID>& Group<T, ID>::append(T* element, ID id)
	{
		auto mp = getGroup(getGroupCount() - 1);
		
		mp->insert(mp->end(), new GroupElement<object_type*, ID>(element, id));
		return *this;
	}
	
	template<class T, typename ID>
	typename Group<T, ID>::mapped_type
	inline& Group<T, ID>::getGroup(Group<T, ID>::key_type& groupName)
	{
		if (hasGroup(groupName))
		{
			return m_group->at(groupName);
		} else
		{
			throw NotFoundException("%s:%d\nThe group %s is not found");
		} // , __FILE__, __LINE__, groupName);
	}
	
	template<class T, typename ID>
	template<class cast_type>
	cast_type*
	Group<T, ID>::get(ID id)
	{
		auto __first = m_group->begin();
		auto __last = m_group->end();
		
		for (auto __iter = __first; __iter != __last; __iter++)
		{
			auto __container = __iter->second;
			for (auto __groupItem : *__container)
			{
				if (__groupItem->equal(id))
				{
					return dynamic_cast<cast_type*>(__groupItem->getElement());
				}
			}
		}
		throw NotFoundException("%s:%d Item with id %d not found."); //, __FILE__, __LINE__, id};
	}
	
	template<class T, typename ID>
	template<class cast_type>
	inline cast_type*
	Group<T, ID>::get(Group<T, ID>::key_type& groupName, ID id)
	{
		if (m_group->find(groupName) == m_group->end())
			throw NotFoundException(string_t("%s:%d Item with id %d not found in group \"%s\""));
		
		
		for (const auto& __groupItem : *m_group->find(groupName)->second)
		{
			if (__groupItem->equal(id))
			{
				return dynamic_cast<cast_type*>(__groupItem->getElement());
			}
		}
		throw NotFoundException(string_t("%s:%d Item with id %d not found in group \"%s\""));
	}
	
	template<class T, typename ID>
	inline size_t
	Group<T, ID>::getGroupCount() const
	{
		return m_group->size();
	}
	
	template<class T, typename ID>
	inline size_t
	Group<T, ID>::getItemCount(Group<T, ID>::key_type& groupName)
	{
		auto __groupItems = getGroup(groupName);
		if (__groupItems->size())
		{
			return getGroup(groupName)->size();
		}
		return 0;
	}
	
	template<class T, typename ID>
	template<typename R, typename ... Args>
	void
	Group<T, ID>::forEach(R(T::*method)(Args ...), Args ... args)
	{
		auto __methodCallback = MethodCallback<T, R, Args ...>(method);
		std::for_each(begin(), end(), [ &__methodCallback, &args ... ](Group<T, ID>::value_type& pair)
		{
			for (element_type* __item : *pair.second)
			{
				if (!__item->empty())
				{
					__methodCallback(__item->getElement(), args...);
				}
			}
		});
	}
	
	template<class T, typename ID>
	template<typename R, typename ... Args>
	void
	Group<T, ID>::forEach(Group<T, ID>::key_type& groupName, R(T::*method)(Args ...), Args ... args)
	{
		auto __methodCallback = MethodCallback<T, R, Args ...>(method);
		for (auto& __item : *(getGroup(groupName)))
		{
			__methodCallback(__item->getElement(), args...);
		}
	}
	
	template<class T, typename ID>
	inline auto
	Group<T, ID>::begin()
	{
		return m_group->begin();
	}
	
	template<class T, typename ID>
	inline auto
	Group<T, ID>::end()
	{
		return m_group->end();
	}
	
	template<class T, typename ID>
	inline const auto
	Group<T, ID>::cbegin() const dxDECL_NOEXCEPT
	{
		return m_group->cbegin();
	}
	
	template<class T, typename ID>
	inline const auto
	Group<T, ID>::cend() const dxDECL_NOEXCEPT
	{
		return m_group->cend();
	}
	
	template<class T, typename ID>
	bool
	Group<T, ID>::hasElement(const Group<T, ID>::key_type& groupName, const ID& id) const
	{
		bool __bExists { };
		
		if (hasGroup(groupName))
		{
			auto __first = m_group->begin();
			auto __last = m_group->end();
			
			for (auto __iterator = __first; __iterator != __last; ++__iterator)
			{
				for (const auto& __element : *__iterator->second)
				{
					if (__element->getProperties()->id == id && __element->getElement() != nullptr)
					{
						__bExists = true;
					}
				}
			}
		}
		return __bExists;
	}
	
	template<class T, typename ID>
	bool Group<T, ID>::hasGroup(const Group<T, ID>::key_type& groupName) const
	{
		for (const value_type& __value : *m_group)
			if (__value.first.compare(groupName) == 0)
			{
				return true;
			}
		
		return false;
	}
}