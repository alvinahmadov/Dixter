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
	namespace Internal
	{
		#ifdef HAVE_CXX17
		using any = std::any;
		
		template<typename ValueType>
		inline ValueType* any_cast(any* op) noexcept
		{
			return ::std::any_cast<ValueType>(op);
		}
		
		template<typename ValueType>
		inline ValueType any_cast(any& op)
		{
			return ::std::any_cast<ValueType>(op);
		}
		
		template<typename ValueType>
		inline ValueType any_cast(const any& op)
		{
			return ::std::any_cast<ValueType>(op);
		}
		
		#else
		using any = ::boost::any;
		
		template<typename ValueType>
		ValueType* any_cast(any* op) noexcept
		{
			return ::boost::any_cast<ValueType>(op);
		}
		
		template<typename ValueType>
		ValueType any_cast(any& op)
		{
			return ::boost::any_cast<ValueType>(op);
		}
		
		template<typename ValueType>
		ValueType any_cast(const any& op)
		{
			return ::boost::any_cast<ValueType>(op);
		}
		
		#endif
	} // namespace Internal
	
	using any_t = Internal::any;
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov%gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \brief Implements default constructible concept.
	 *
	 * Derived classes will have a default ctor. Implements C++ version-dependent
	 * default construction.
	 * */
	struct DefaultConstructible
	{
	protected:
		DefaultConstructible() = default;
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
	struct CopyConstructible
	{
	protected:
		CopyConstructible() noexcept = default;
		
		virtual ~CopyConstructible() noexcept = default;
		
		CopyConstructible(const CopyConstructible&) noexcept = default;
		
		CopyConstructible& operator=(const CopyConstructible&) noexcept = default;
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
	struct Movable
	{
	protected:
		constexpr Movable() noexcept = default;
		
		virtual ~Movable() noexcept = default;
		
		Movable(Movable&&) noexcept = default;
		
		Movable& operator=(Movable&&) noexcept = default;
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
	struct NonCopyable
	{
	protected:
		NonCopyable() = default;
		
		virtual ~NonCopyable() = default;
		
		NonCopyable(NonCopyable&& other) = default;
		
		NonCopyable& operator=(NonCopyable&& other) = default;
		
		NonCopyable(const NonCopyable&) = delete;
		
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
	
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
		virtual int compareTo(const T& otherComparable) noexcept = 0;
	};
	
	template<class T>
	class ClassInfo : public IComparable<ClassInfo<T>>
	{
	public:
		using TClass    = T;
		using TClassRef = T&;
		using TClassPtr = T*;
		
		virtual ~ClassInfo() noexcept = default;
		
		static TString getName();
		
		static constexpr
		TByteArray getRawName();
		
		template<class Other>
		static constexpr
		bool compare();
		
		/**
		 * \link ComparableInterface::compareTo() \endlink
		 * */
		int compareTo(const ClassInfo<T>& other) noexcept override;
		
		static constexpr
		const TSize& getSize();
	
	private:
		ClassInfo();
		
		static std::unique_ptr<ClassInfo>& getInstance();
		
		/**
		 * \author Alvin Ahmadov
		 * \namespace Dixter
		 * \class ClassNameConvertor
		 * \brief Represents C++ and user-defined types as string.
		 * */
		class ClassNameConvertor
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
		
		static std::unique_ptr<ClassInfo> m_classInstance;
	};
	
	class StopWatch final
	{
		using SystemClock = std::chrono::system_clock;
	public:
		StopWatch() noexcept
				: m_time(SystemClock::now())
		{ }
		
		~StopWatch() noexcept
		{
			if (m_showOnDelete)
				printf("\nOverall run time %.3lf s.\n", elapsed(true));
		}
		
		inline Real32 elapsed(bool showOnDelete = false);
	
	private:
		bool m_showOnDelete = true;
		
		SystemClock::time_point m_time;
	};
	
	/**
	 * \author Alvin Ahmadov
	 * \namespace Dixter
	 *
	 * \brief Class used to format passed variable arguments to readable form.
	 *
	 * \tparam Args Argument types to be passed to formatter.
	 * */
	template<typename... Args>
	class VarArgMessageFormat
	{
		using Format = boost::basic_format<TString::value_type>;
		
		/**
		 * \brief Check if \c arg's is of type T.
		 * \tparam T Type to check.
		 * \returns True if \c T is \c arg's type.
		 * */
		template<typename T>
		static constexpr
		bool checkTypeIdentity(const any_t& arg);
		
		/**
		 * \brief Casts argument to its original type and appends to string.
		 * \tparam T Type to cast argument.
		 * \param fmt boost::format pointer.
		 * \param arg Object of type any to be cast and appended.
		 * */
		template<typename T>
		static void append(Format* fmt, const any_t& arg);
	
	public:
		
		/**
		 * \brief Public interface for class format operations.
		 * \param message String to which add arguments.
		 * \param args Arguments to be appended to \c message.
		 * */
		static void format(TString& message, Args&& ... args);
	};
	
	/// ClassInfo<> implementation
	template<class T>
	std::unique_ptr<ClassInfo<T>> ClassInfo<T>::m_classInstance = nullptr;
	
	template<class T> TSize
			ClassInfo<T>::m_size {};
	
	template<class T>
	ClassInfo<T>::ClassInfo()
			: m_className(typeid(T).name())
	{
		#ifdef HAVE_CXX17
		if constexpr (not std::is_null_pointer<TClass>::value)
		#else
		if (not std::is_null_pointer<T>::m_value)
			#endif
			m_size = sizeof(TClass);
		else
			m_size = 0;
	}
	
	template<class T>
	inline std::unique_ptr<ClassInfo<T>>&
	ClassInfo<T>::getInstance()
	{
		if (not m_classInstance)
			m_classInstance = std::unique_ptr<ClassInfo>(new ClassInfo);
		
		return m_classInstance;
	}
	
	template<class T>
	inline TString ClassInfo<T>::getName()
	{
		return ClassNameConvertor::getName(getRawName());
	}
	
	template<class T>
	inline constexpr const char*
	ClassInfo<T>::getRawName()
	{
		return getInstance()->m_className;
	}
	
	template<class T>
	template<class Other>
	inline constexpr bool
	ClassInfo<T>::compare()
	{
		#ifdef HAVE_CXX17
		return std::is_same_v<T, Other>;
		#else
		return std::is_same<T, Other>::m_value;
		#endif
	}
	
	template<class T>
	inline Int32
	ClassInfo<T>::compareTo(const ClassInfo<T>& other) noexcept
	{
		return ( getSize() == other.getSize()
		         ? 0
		         : ( getSize() > other.getSize())
		           ? 1
		           : -1 );
	}
	
	template<class T>
	inline constexpr
	const TSize& ClassInfo<T>::getSize()
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
	
	inline Real32 StopWatch::elapsed(bool showOnDelete)
	{
		m_showOnDelete = showOnDelete;
		return (SystemClock::now() - m_time).count() * TRatioDivider<std::nano>::value;
	}
	
	template<typename... Args>
	inline void VarArgMessageFormat<Args...>::format(TString& message, Args&& ... args)
	{
		auto __pFmt = new Format(message);
		std::initializer_list<any_t> __argumentsAny { std::forward<Args>(args)..., sizeof...(Args) };
		
		for (auto& __anyArg : __argumentsAny)
		{
			append<TString>(__pFmt, __anyArg);
			append<TUString>(__pFmt, __anyArg);
			append<const char*>(__pFmt, __anyArg);
			append<int>(__pFmt, __anyArg);
			append<double>(__pFmt, __anyArg);
			append<unsigned int>(__pFmt, __anyArg);
		}
		message.clear();
		message = __pFmt->str();
		delete __pFmt;
	}
	
	template<typename... Args>
	template<typename T>
	inline constexpr
	bool VarArgMessageFormat<Args...>::checkTypeIdentity(const any_t& anyArg)
	{
		return typeid(T) == anyArg.type();
	}
	
	template<typename... Args>
	template<typename T>
	inline void VarArgMessageFormat<Args...>::append(Format* fmt, const any_t& arg)
	{
		if (checkTypeIdentity<T>(arg))
			( *fmt ) % Internal::any_cast<T>(arg);
	}
} // namespace Dixter


#ifdef PERFORMANCE_TEST
#   define dxTIMER_START StopWatch __stopWatch;
#   define dxTIMER_STOP  printf("\nOverall run time %.3lf s.\n", __stopWatch.elapsed());
#else
#   define dxTIMER_START
#   define TIMER_STOP
#endif