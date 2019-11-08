/**
 *  File Common.h
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
 
#pragma once

#include <functional>
#include <boost/format.hpp>
#include <map>
#include <list>
#include <chrono>
#include <memory>
#include "Types.hpp"

#ifdef HAVE_CXX17

#include <any>

#else

#include <boost/any.hpp>

#endif

namespace Dixter
{
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 * \ingroup base
	 * \interface ComparableInterface
	 * \brief Used to indicate that object is comparable.
	 * \tparam T Type of object that implements comparable interface.
	 * */
	template<class T>
	struct IComparable
	{
		/**
		 * \interface ComparableInterface
		 * \brief Compare objects.
		 * \param otherComparable Object to compare with this object.
		 *
		 * Compares two elements of type T. Returns -1 if \c otherComparable
		 * is greater than \c this, 0 if objects equal, 1 otherwise.
		 * */
		virtual int compareTo(const T& comparable) noexcept = 0;
	};
	
	// Concepts
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov%gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \brief Implements default constructible concept.
	 *
	 * Derived classes will have a default ctor. Implements C++ version-dependent
	 * default construction.
	 * */
	struct TDefaultConstructible
	{
	protected:
		TDefaultConstructible() noexcept = default;
		
		~TDefaultConstructible() noexcept = default;
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \class Copyable
	 * \brief Sets default copying concept.
	 *
	 * Derived classes by default can be copied. Implements C++ version-dependent
	 * copying.
	 * */
	struct TCopyConstructible
	{
	private:
		using TSelf = TCopyConstructible;
		
	protected:
		TCopyConstructible() noexcept = default;
		
		virtual ~TCopyConstructible() noexcept = default;
		
		TCopyConstructible(const TCopyConstructible&) noexcept = default;
		
		TCopyConstructible& operator=(const TCopyConstructible&) noexcept = default;
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \class NonCopyable
	 * \brief Prohibits object to be copied.
	 *
	 * Derived classes can't be copied. Implements C++ version-dependent
	 * copying restriction.
	 * */
	struct TNonCopyable : public TDefaultConstructible
	{
	private:
		using TSelf = TNonCopyable;
		
	protected:
		TNonCopyable() noexcept = default;
		
		virtual ~TNonCopyable() noexcept = default;
		
		TNonCopyable(TSelf&& other) noexcept = default;
		
		TSelf& operator=(TSelf&& other) noexcept = default;
		
		TNonCopyable(const TSelf&) = delete;
		
		TSelf& operator=(const TSelf&) = delete;
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov%gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \class Movable
	 * \brief Implements move and assignable concept.
	 *
	 * Derived classes will have a default move ctor. Implements C++ version-dependent
	 * default move ctor.
	 * */
	struct TMovable
	{
	private:
		using TSelf = TMovable;
		
	protected:
		constexpr TMovable() noexcept = default;
		
		virtual ~TMovable() noexcept = default;
		
		TMovable(TSelf&&) noexcept = default;
		
		TSelf& operator=(TSelf&&) noexcept = default;
	};
	
	struct TMoveOnly : public TNonCopyable
	{
	private:
		using TSelf = TMoveOnly;
		
	protected:
		constexpr TMoveOnly() noexcept = default;
		
		virtual ~TMoveOnly() noexcept = default;
		
		TMoveOnly(TSelf&&) noexcept = default;
		
		TSelf& operator=(TSelf&&) noexcept = default;
	};
	
	template<class TUnderlying>
	class TClassInfo : public IComparable<TClassInfo<TUnderlying>>,
					   public TMoveOnly
	{
	public:
		using TSelf = TClassInfo<TUnderlying>;
		
		virtual ~TClassInfo() noexcept = default;
		
		static TString getName();
		
		static constexpr
		TByteArray getRawName();
		
		template<class Other>
		static constexpr
		bool compare();
		
		/**
		 * \link ComparableInterface::compareTo() \endlink
		 * */
		int compareTo(const TSelf& other) noexcept override;
		
		static constexpr
		const TSize& getSize();
	
	private:
		TClassInfo() noexcept;
		TClassInfo(TSelf&&) noexcept = default;
		TSelf& operator=(TSelf&&) noexcept = default;
		
		static std::unique_ptr<TSelf>& getInstance();
		
		/**
		 * \author Alvin Ahmadov
		 * \namespace Dixter
		 * \class ClassNameConvertor
		 * \brief Represents C++ and user-defined types as string.
		 * */
		class TClassNameConvertor
		{
		public:
			/**
			 * \returns string name of the class
			 * */
			static TString getName(const TString& rawName);
			
			/**
			 * \see \code getName(const string_t&)
			 * */
			static TString getName(const TByte* rawName);
			
			/**
			 * \brief Parses raw class name to formatted one.
			 * \see \code getName(const string_t&)
			 * */
			static TString parseName(const TByte* raw);
		
		private:
			static std::map<TString, TString> m_names;
			
			static std::list<TByteArray> m_charTable;
			
			static const TSize m_charTableSize;
		};
	
	private:
		static TSize m_size;
		
		TByteArray m_className;
		
		static std::unique_ptr<TSelf> m_classInstance;
	};
	
	class TStopWatch final
	{
		using TClock = std::chrono::system_clock;
	public:
		TStopWatch() noexcept
				: m_time(TClock::now())
		{ }
		
		~TStopWatch() noexcept
		{
			if (m_showOnDelete)
				printf("\nOverall run time %.3lf s.\n", elapsed(true));
		}
		
		inline Real32 elapsed(bool showOnDelete = false);
	
	private:
		bool m_showOnDelete = true;
		
		TClock::time_point m_time;
	};
	
	namespace NInternal
	{
		#ifdef HAVE_CXX17
		using TAny = std::any;
		
		template<typename TValue>
		inline TValue* anyCast(TAny* op) noexcept
		{
			return ::std::any_cast<TValue>(op);
		}
		
		template<typename TValue>
		inline TValue anyCast(TAny& op)
		{
			return ::std::any_cast<TValue>(op);
		}
		
		template<typename TValue>
		inline TValue anyCast(const TAny& op)
		{
			return ::std::any_cast<TValue>(op);
		}
		
		#else
		using TAny = ::boost::any;
		
		template<typename TValue>
		TValue* anyCast(TAny* op) noexcept
		{
			return ::boost::any_cast<TValue>(op);
		}
		
		template<typename TValue>
		TValue anyCast(TAny& op)
		{
			return ::boost::any_cast<TValue>(op);
		}
		
		template<typename TValue>
		TValue anyCast(const TAny& op)
		{
			return ::boost::any_cast<TValue>(op);
		}
		
		#endif
	} // namespace Internal
	
	using TAny = NInternal::TAny;
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 *
	 * \brief Class used to format passed variable arguments to readable form.
	 *
	 * \tparam Args Argument types to be passed to formatter.
	 * */
	template<typename... TArgs>
	class TVarArgMessageFormat
	{
		using TSelf 	= TVarArgMessageFormat;
		using TFormat 	= boost::basic_format<TString::value_type>;
		
		/**
		 * \brief Check if \c arg's is of type T.
		 * \tparam T Type to check.
		 * \returns True if \c T is \c arg's type.
		 * */
		template<typename TUnderlying>
		static constexpr
		bool checkTypeIdentity(const TAny& arg)
		{
			return typeid(TUnderlying) == arg.type();
		}
		
		/**
		 * \brief Casts argument to its original type and appends to string.
		 * \tparam T Type to cast argument.
		 * \param fmt boost::format pointer.
		 * \param arg Object of type any to be cast and appended.
		 * */
		template<typename TUnderlying>
		static void append(TFormat* fmt, const TAny& arg)
		{
			if (TSelf::checkTypeIdentity<TUnderlying>(arg))
				( *fmt ) % NInternal::anyCast<TUnderlying>(arg);
		}
	
	public:
		
		/**
		 * \brief Public interface for class format operations.
		 * \param message String to which add arguments.
		 * \param args Arguments to be appended to \c message.
		 * */
		static void format(TString& message, TArgs&&... args);
	};
	
	// TClassInfo<> implementation
	template<class T>
	std::unique_ptr<TClassInfo<T>>
	TClassInfo<T>::m_classInstance = nullptr;
	
	template<class T> TSize
			TClassInfo<T>::m_size {};
	
	template<typename TUnderlying>
	TClassInfo<TUnderlying>::TClassInfo() noexcept
			: m_className(typeid(TUnderlying).name())
	{
		#ifdef HAVE_CXX17
		if constexpr (not std::is_null_pointer<TUnderlying>::value)
		#else
		if (not std::is_null_pointer<T>::m_value)
		#endif
			m_size = sizeof(TUnderlying);
		else
			m_size = 0;
	}
	
	template<typename TUnderlying>
	inline std::unique_ptr<TClassInfo<TUnderlying>>&
	TClassInfo<TUnderlying>::getInstance()
	{
		if (not m_classInstance)
			m_classInstance.reset(new TSelf);
		
		return m_classInstance;
	}
	
	template<class T>
	inline TString TClassInfo<T>::getName()
	{
		return TClassNameConvertor::getName(getRawName());
	}
	
	template<class T>
	inline constexpr const char*
	TClassInfo<T>::getRawName()
	{
		return getInstance()->m_className;
	}
	
	template<class T>
	template<class Other>
	inline constexpr bool
	TClassInfo<T>::compare()
	{
		#ifdef HAVE_CXX17
		return std::is_same_v<T, Other>;
		#else
		return std::is_same<T, Other>::m_value;
		#endif
	}
	
	template<class T>
	inline Int32
	TClassInfo<T>::compareTo(const TClassInfo<T>& other) noexcept
	{
		return ( getSize() == other.getSize()
		         ? 0
		         : ( getSize() > other.getSize())
		           ? 1
		           : -1 );
	}
	
	template<class T>
	inline constexpr const TSize&
	TClassInfo<T>::getSize()
	{
		return getInstance()->m_size;
	}
	
	template<
	        typename TRatio,
			typename TReturn = Real32
	>
	struct TRatioDivider
	{
		constexpr static TReturn value = static_cast<TReturn>(TRatio::num)/static_cast<TReturn>(TRatio::den);
	};
	
	inline Real32 TStopWatch::elapsed(bool showOnDelete)
	{
		m_showOnDelete = showOnDelete;
		return (TClock::now() - m_time).count() * TRatioDivider<std::nano>::value;
	}
	
	template<typename... TArgs>
	inline void TVarArgMessageFormat<TArgs...>::
	format(TString& message, TArgs&& ... args)
	{
		auto __pFmt = std::unique_ptr<TFormat>(new TFormat(message));
		std::initializer_list<TAny> __argumentsAny { std::forward<TArgs>(args)..., sizeof...(TArgs) };
		
		for (auto& __anyArg : __argumentsAny)
		{
			TSelf::append<TString>(__pFmt.get(), __anyArg);
			TSelf::append<TUString>(__pFmt.get(), __anyArg);
			TSelf::append<const
				   TByte*>(__pFmt.get(), __anyArg);
			TSelf::append<Int32>(__pFmt.get(), __anyArg);
			TSelf::append<Real32>(__pFmt.get(), __anyArg);
			TSelf::append<UInt32>(__pFmt.get(), __anyArg);
		}
		message.clear();
		message = __pFmt->str();
	}
} // namespace Dixter


#ifdef PERFORMANCE_TEST
#   define dxTIMER_START TStopWatch __stopWatch;
#   define dxTIMER_STOP  printf("\nOverall run time %.3lf s.\n", __stopWatch.elapsed());
#	define dxTIMER_STOPL printf("\n%s(): Finished for %.3lf s.\n", __FUNCTION__, __stopWatch.elapsed());
#else
#   define dxTIMER_START
#   define TIMER_STOP
#endif