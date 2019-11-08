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
	template<class T, typename ID = UInt32>
	class TGroup final : public TNonCopyable
	{
	public:
		using TObject               = T;
		using EId                   = ID;
		using TKey                  = TString;
		using TElement              = TGroupElement<TObject*, ID>;
		using TContainer            = std::list<TElement*>;
		using TMapped               = TContainer*;
		using TValue                = std::pair<TKey, TMapped>;
		using TMultimap             = std::unordered_map<TKey, TMapped>;
		using TIterator             = typename TGroup<T, ID>::TMultimap::iterator;
		using TConstIterator        = typename TGroup<T, ID>::TMultimap::const_iterator;
		using TListIterator         = typename std::list<TElement*>::iterator;
		using TListConstIterator    = const typename std::list<TElement*>::iterator;
	public:
		TGroup() noexcept;
		
		TGroup(TGroup&& src) noexcept;
		
		TGroup& operator=(TGroup&& src) noexcept;
		
		~TGroup() noexcept;
		
		/**
		 * \class Group
		 * \brief Add element to the group and return casted pointer to that element.
		 * \tparam TCast Type of added element.
		 * \param groupName The name of the group to be added.
		 * \param element Element to be added to the group.
		 * \param id Id associated with the element.
		 * \param autoDelete Automatical deletion of element if unused.
		 * \param description Description of the element (optional).
		 * */
		template<class TCast = TObject>
		TCast*
		add(const TKey& groupName, TObject* element, ID id, bool autoDelete = false,
			typename TElement::TDescription description = "");
		
		/**
		 * \class Group
		 * \brief Add element to the group and return casted pointer to that element.
		 * \tparam TCast Type of added element.
		 * \param groupName The name of the group to be added.
		 * \param element Element to be added to the group.
		 * \param id Id associated with the element.
		 * \param autoDelete Automatical deletion of element if unused.
		 * \param description Description of the element (optional).
		 * */
		template<class TCast = TObject>
		TCast*
		add(TObject* element, ID id, bool autoDelete = false,
			typename TElement::TDescription description = "");
		
		/**
		 * \class Group
		 * \brief Remove element in group groupName with matching id.
		 * \param groupName The name of the group in which located the element.
		 * \param id Id of element to remove.
		 * \returns True if element removed successfully.
		 * */
		bool remove(const TKey& groupName, ID id);
		
		void clear();
		
		TGroup& append(TObject* element, ID id);
		
		/**
		 * \class Group
		 * \brief Finds elements of a group.
		 * \param groupName The name of the group.
		 * \returns Elements of the group.
		 * \throws NotFoundException
		 * */
		TMapped& getGroup(const TKey& groupName);
		
		TMapped& getGroup(TSize index)
		{
			TSize idx {};
			
			for (std::pair<TKey, TMapped>& g : *m_group)
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
		 * \tparam TCast Type to cast the element.
		 * \param id Id of the element.
		 * \returns Casted element.
		 * */
		template<class TCast = TObject>
		TCast*
		get(ID id);
		
		/**
		 * \class Group
		 * \brief Get element.
		 * \tparam TCast Type to cast the element.
		 * \param groupName Name of the group.
		 * \param id Id of the element.
		 * \returns Casted element.
		 * \throws NotFoundException
		 * */
		template<class TCast = TObject>
		TCast*
		get(const TKey& groupName, ID id);
		
		/**
		 * \class Group
		 * \brief Calls specified method of element.
		 * \tparam TReturn Return type of element's method.
		 * \tparam TArgs Types of variadic arguments to be passed to the method.
		 * \param args Arguments for method.
		 *
		 * For each element of all groups call method. All groups must have
		 * the same types.
		 * */
		template<
				typename TReturn,
				typename... TArgs
		>
		void forEach(TReturn(T::* MethodCallback)(TArgs...), TArgs ... args);
		
		/**
		 * \class Group
		 * \brief Calls specified method of element.
		 * \tparam TReturn Return type of element's method.
		 * \tparam TArgs Types of variadic arguments to be passed to the method.
		 * \param groupName Name of the group.
		 * \param args Arguments for method.
		 *
		 * For each element of a group calls its method.
		 * */
		template<
				typename TReturn,
				typename... TArgs
		>
		void forEach(const TKey& groupName, TReturn(T::*MethodCallback)(TArgs ... args), TArgs ... args);
		
		/**
		 * \class Group
		 * \returns Number of groups.
		 * */
		TSize getSize() const;
		
		/**
		 * \class Group
		 * \param groupName Name of the group.
		 * \returns Number of elements in the group.
		 * */
		TSize getItemCount(const TKey& groupName) const;
		
		bool empty() const noexcept;
		
		bool empty(const TKey& key) const noexcept;
		
		bool isEmpty() const noexcept;
		
		bool isEmpty(const TKey& key) const noexcept;
		
		TIterator
		begin() noexcept;
		
		TIterator
		end() noexcept;
		
		TConstIterator
		cbegin() const noexcept;
		
		TConstIterator
		cend() const noexcept;
	
	private:
		/**
		 * \class Group
		 * \brief Insert a GroupElement object to the group.
		 * \param groupName The name of the group to be added.
		 * \param groupElement The name of the element to be added to group.
		 * \returns True if item added successfully.
		 * \throws IllegalArgumentException.
		 */
		TObject* doAdd(const TKey& groupName, TElement* groupElement);
		
		bool
		hasElement(const TKey& groupName, const ID& id) const;
		
		/*! Method that finds item in group by its group name
		 * @param groupName Name of the group where exists the searched data
		 * @returns A pair with true if found, and the iterator to the item
		 */
		bool
		hasGroup(const TKey& groupName) const;
		
		/**
		 * \class Group
		 * \brief Checks if group exists or group name isn't empty.
		 * \param groupName Name of group to check.
		 * \returns True if group is valid.
		 * */
		inline bool
		isValidGroupName(const TKey& groupName) const;
		
		/**
		 * \class Group
		 * \brief Checks if element isn't null.
		 * \param element Element to check.
		 * \returns True if element is valid.
		 * */
		inline bool
		isValidElement(const TObject* element) const;
	
	private:
		TSize m_count;
		
		TMultimap* m_group;
		
		static const TKey s_defaultGroup;
		
		mutable std::mutex m_mutex;
	};
} // namespace Dixter

#include "GroupImpl.hpp"
