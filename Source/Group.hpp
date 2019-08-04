/**
 *  Copyright (C) 2013-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#include "GroupElement.hpp"
#include "MethodCallback.hpp"
#include <cassert>
#include <mutex>

namespace Dixter
{
	
	/*!
	 * \author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
	 * \class Group
	 * \namespace Dixter
	 * \extends AbstractGroup, NonCopyable
	 * \brief Group is a container storing objects which have common name.
	 *
	 * It can store objects of classes which have common base class.
	 * */
	template<class T, typename ID = ui32>
	class Group final : public NonCopyable
	{
	public:
		using object_type           = T;
		using id_type               = ID;
		using key_type              = const string_t;
		using element_type          = GroupElement<object_type*, ID>;
		using container_type        = std::list<element_type*>;
		using mapped_type           = container_type*;
		using value_type            = std::pair<key_type, mapped_type>;
		using multimap_type         = std::unordered_map<key_type, mapped_type, DJBHash>;
		using iterator              = typename Group<T, ID>::multimap_type::iterator;
		using const_iterator        = typename Group<T, ID>::multimap_type::const_iterator;
		using list_iterator         = typename std::list<element_type*>::iterator;
		using list_const_iterator   = const typename std::list<element_type*>::iterator;
	public:
		/// ctor.
		/// Initialises group with empty data
		Group();
		
		/// Move ctor.
		Group(Group&& src);
		
		Group(const Group&) = delete;
		
		Group& operator=(const Group&) = delete;
		
		/// dtor.
		~Group();
		
		/**
		 * \class Group
		 * \brief Add element to the group and return casted pointer to that element.
		 * \tparam cast_type Type of added element.
		 * \param groupName The name of the group to be added.
		 * \param element Element to be added to the group.
		 * \param id Id associated with the element.
		 * \param autoDelete Automatical deletion of element if unused.
		 * \param description Description of the element (optional).
		 * */
		template<class cast_type = object_type>
		cast_type*
		add(key_type& groupName, T* element, ID id, bool autoDelete = false, const string_t& description = string_t());
		
		/**
		 * \class Group
		 * \brief Add element to the group and return casted pointer to that element.
		 * \tparam cast_type Type of added element.
		 * \param groupName The name of the group to be added.
		 * \param element Element to be added to the group.
		 * \param id Id associated with the element.
		 * \param autoDelete Automatical deletion of element if unused.
		 * \param description Description of the element (optional).
		 * */
		template<class cast_type = object_type>
		cast_type*
		add(T* element, ID id, bool autoDelete = false, const string_t& description = string_t());
		
		/**
		 * \class Group
		 * \brief Remove element in group groupName with matching id.
		 * \param groupName The name of the group in which located the element.
		 * \param id Id of element to remove.
		 * \returns True if element removed successfully.
		 * */
		bool remove(key_type& groupName, ID id);
		
		Group& append(T* element, ID id);
		
		/**
		 * \class Group
		 * \brief Finds elements of a group.
		 * \param groupName The name of the group.
		 * \returns Elements of the group.
		 * \throws NotFoundException
		 * */
		mapped_type& getGroup(key_type& groupName);
		
		mapped_type& getGroup(size_t index)
		{
			size_t idx { };
			
			for (std::pair<key_type, mapped_type>& g : *m_group)
			{
				if (idx++ == index)
				{
					return g.second;
				}
			}
		}
		
		/**
		 * \class Group
		 * \brief Get element.
		 * \tparam cast_type Type to cast the element.
		 * \param id Id of the element.
		 * \returns Casted element.
		 * */
		template<class cast_type = object_type>
		cast_type*
		get(ID id);
		
		/**
		 * \class Group
		 * \brief Get element.
		 * \tparam cast_type Type to cast the element.
		 * \param groupName Name of the group.
		 * \param id Id of the element.
		 * \returns Casted element.
		 * \throws NotFoundException
		 * */
		template<class cast_type = object_type>
		cast_type*
		get(key_type& groupName, ID id);
		
		/**
		 * \class Group
		 * \returns Number of groups.
		 * */
		size_t getGroupCount() const;
		
		/**
		 * \class Group
		 * \param groupName Name of the group.
		 * \returns Number of elements in the group.
		 * */
		size_t getItemCount(key_type& groupName);
		
		/**
		 * \class Group
		 * \brief Calls specified method of element.
		 * \tparam R Return type of element's method.
		 * \tparam Args Types of variadic arguments to be passed to the method.
		 * \param args Arguments for method.
		 *
		 * For each element of all groups call method. All groups must have
		 * the same types.
		 * */
		template<typename R, typename ... Args>
		void forEach(R(T::* MethodCallback)(Args...), Args ... args);
		
		/**
		 * \class Group
		 * \brief Calls specified method of element.
		 * \tparam R Return type of element's method.
		 * \tparam Args Types of variadic arguments to be passed to the method.
		 * \param groupName Name of the group.
		 * \param args Arguments for method.
		 *
		 * For each element of a group calls its method.
		 * */
		template<typename R, typename ... Args>
		void forEach(key_type& groupName, R(T::*MethodCallback)(Args ... args), Args ... args);
		
		auto begin();
		
		auto end();
		
		const auto cbegin() const dxDECL_NOEXCEPT;
		
		const auto cend() const dxDECL_NOEXCEPT;
		
		/**
		 * \class Group
		 * \brief Checks if group exists or group name isn't empty.
		 * \param groupName Name of group to check.
		 * \returns True if group is valid.
		 * */
		inline dxDECL_CONSTEXPR bool validGroupName(const key_type& groupName)
		{
			return hasGroup(groupName) || groupName.size() > 0;
		}
		
		/**
		 * \class Group
		 * \brief Checks if element isn't null.
		 * \param element Element to check.
		 * \returns True if element is valid.
		 * */
		inline dxDECL_CONSTEXPR bool validElement(object_type* element)
		{
			return element != nullptr;
		}
		
		Group& operator()(key_type& groupName, object_type* item, ID id, bool autoRelease = true)
		{
			add(groupName, item, id, ClassInfo<T>::getRawName(), autoRelease);
			return *this;
		}
		
		bool hasElement(const key_type& groupName, const ID& id) const;
		
		/*! Method that finds item in group by its group name
		 * @param groupName Name of the group where exists the searched data
		 * @returns A pair with true if found, and the iterator to the item
		 */
		bool hasGroup(const key_type& groupName) const;
	
	protected:
		/**
		 * \class Group
		 * \brief Insert a GroupElement object to the group.
		 * \param groupName The name of the group to be added.
		 * \param groupElement The name of the element to be added to group.
		 * \returns True if item added successfully.
		 * \throws IllegalArgumentException.
		 */
		object_type* add(key_type& groupName, Group<T, ID>::element_type* groupElement);
	
	private:
		size_t m_count;
		
		multimap_type* m_group;
		
		mutable std::mutex m_mutex;
	};
}

#include "GroupImpl.hpp"