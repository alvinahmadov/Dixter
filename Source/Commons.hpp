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
#include <exception>
#include <map>
#include <list>
#include "Macros.hpp"
#include "Types.hpp"
#ifdef BENCHMARK_TEST
#include <boost/timer.hpp>
#endif

#ifdef HAVE_CXX17

#include <any>

#else
#include <boost/any.hpp>
#endif

namespace Dixter
{
#ifdef BENCHMARK_TEST
	#define BENCH_BEGIN auto t = ::boost::timer {};
	#define BENCH_END   printf("Overall run time %lf seconds.", t.elapsed());
#else
	#define BENCH_BEGIN
	#define BENCH_END
#endif
	
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
	}
	
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
	struct Constructible
	{
		#ifdef HAVE_CXX11
		/// Default ctor.
		Constructible() = default;
		
		#else
		Constructible() {};
		#endif
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
	struct Copyable
	{
		/// Default copy ctor.
		Copyable(const Copyable&) = default;
		
		/// Default copy operator.
		Copyable& operator=(const Copyable&) = default;
	};
	
	struct CopyConstructible
	{
		CopyConstructible() = default;
		/// Default copy ctor.
		CopyConstructible(const CopyConstructible&) = default;
		
		/// Default copy operator.
		CopyConstructible& operator=(const CopyConstructible&) = default;
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
		#ifdef HAVE_CXX11
		
		Movable() = default;
		
		Movable(Movable&&) = default;
		
		#else
		Movable() {};
		
		Movable(Movable&&);
		#endif
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
		#ifdef HAVE_CXX11
		/// Default ctor.
		NonCopyable() = default;
		
		/// Deleted copy ctor.
		NonCopyable(const NonCopyable&) = delete;
		
		/// Deleted copy operator.
		NonCopyable& operator=(const NonCopyable&) = delete;
		
		#else
		public:
		/// Public default ctor.
			NonCopyable() {};
			
		private:
		/// Private copy ctor
			NonCopyable(const NonCopyable &src);
			
		/// Private copy operator.
			NonCopyable &operator=(const NonCopyable &src);
		#endif
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov%gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \class DefaultNonCopyable
	 * \brief Implements default ctor and non-copyable concept.
	 *
	 * Derived classes will have a default and non-copyable ctors. Implements C++ version-dependent
	 * default ctor and non-copyable ctors.
	 * */
	struct DefaultNonCopyable : public Constructible
	{
		#ifdef HAVE_CXX11
		/// Default ctor.
		DefaultNonCopyable() = default;
		
		/// Deleted copy ctor.
		DefaultNonCopyable(const DefaultNonCopyable&) = delete;
		
		/// Deleted copy operator.
		DefaultNonCopyable& operator=(const DefaultNonCopyable&) = delete;
		
		#else
		public:
		DefaultNonCopyable() {};
		private:
			DefaultNonCopyable(const DefaultNonCopyable &src);
			
			DefaultNonCopyable &operator=(const DefaultNonCopyable &src);
		#endif
	};
	
	/**
	 * \author Alvin Ahmadov <alvin.dev.ahmadov%gmail.com>
	 * \namespace Dixter
	 * \ingroup base
	 * \class NonDefault
	 * \brief Implements deleted default concept.
	 *
	 * Derived classes will not have a default ctor. Implements C++ version-dependent
	 * deleted default ctor.
	 * */
	struct NonDefault
	{
		#ifdef HAVE_CXX11
		NonDefault() = delete;
		
		#else
		private:
			NonDefault() {};
		#endif
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
	struct ComparableInterface
	{
		/**
		 * \interface ComparableInterface
		 * \brief Compare objects.
		 * \param otherComparable Object to compare with this object.
		 *
		 * Compares two elements of type T. Returns -1 if \c otherComparable
		 * is greater than \c this, 0 if objects equal, 1 otherwise.
		 * */
		virtual int compareTo(const T& otherComparable) = 0;
	};
	
	template<class T>
	class ClassInfo : public ComparableInterface<ClassInfo<T>>
	{
	public:
		using underlying_class_t = T;
	
	public:
		virtual ~ClassInfo();
		
		static string_t getName();
		
		static const char* getRawName();
		
		template<class Other>
		constexpr static bool compare();
		
		/**
		 * \link ComparableInterface::compareTo() \endlink
		 * */
		constexpr int compareTo(const ClassInfo<T>& other) dxDECL_OVERRIDE;
		
		static const size_t& getSize();
	
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
			static string_t getName(const string_t& rawName);
			
			/**
			 * \see \code getName(const string_t&)
			 * */
			static string_t getName(const char* rawName);
			
			/**
			 * \brief Parses raw class name to formatted one.
			 * \see \code getName(const string_t&)
			 * */
			static string_t parseName(const char* raw);
		
		private:
			static string_t _getName(const char* rawName);
		
		private:
			static std::map<string_t, string_t> m_names;
			static std::list<const byte*> m_charTable;
			static const size_t m_charTableSize;
		};
	
	private:
		static size_t s_size;
		const_bytes m_className;
		static std::unique_ptr<ClassInfo> m_classInstance;
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
		using Format = boost::basic_format<typename string_t::value_type>;
		
		/**
		 * \brief Check if \c arg's is of type T.
		 * \tparam T Type to check.
		 * \returns True if \c T is \c arg's type.
		 * */
		template<typename T>
		static constexpr bool checkTypeIdentity(const any_t& arg);
		
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
		static void format(string_t& message, Args&& ... args);
	};
	
	//TODO: Extend exception class to have a stackTrace functionality and detailed information.
	class Exception : public std::exception
	{
	public:
		explicit Exception(const string_t& message = string_t()) dxDECL_NOEXCEPT;
		
		virtual ~Exception() dxDECL_NOEXCEPT dxDECL_OVERRIDE;
		
		virtual const char* what() const dxDECL_NOEXCEPT dxDECL_OVERRIDE;
		
		virtual const string_t& getMessage() const dxDECL_NOEXCEPT;
	
	protected:
		string_t m_message;
	};
	
	#define DECL_DETAILED_EXCEPTION(className)                       \
    class className : public Exception                               \
    {                                                                \
    public:                                                          \
        explicit className(const string_t& message = string_t());    \
        template <typename ... Args>                                 \
        explicit className(string_t&& message, Args&& ... args)      \
            : Exception()                                            \
            {                                                        \
               VarArgMessageFormat<Args...>::                        \
                       format(message,                               \
                       		std::forward<Args>(args)...);            \
               m_message = message;        		                     \
            };                                                       \
        template <typename ... Args>                                 \
        explicit className(const char* message, Args&& ... args)     \
            : className(string_t(message), args...)                  \
            {};                                                      \
        ~className() dxDECL_NOEXCEPT dxDECL_OVERRIDE;                \
        const char* what() const dxDECL_NOEXCEPT dxDECL_OVERRIDE;    \
        const string_t& getMessage() const dxDECL_NOEXCEPT           \
        dxDECL_OVERRIDE;                                             \
    };
	
	DECL_DETAILED_EXCEPTION(NotImplementedException)
	
	DECL_DETAILED_EXCEPTION(IllegalArgumentException)
	
	DECL_DETAILED_EXCEPTION(NullPointerException)
	
	DECL_DETAILED_EXCEPTION(NotFoundException)
	
	DECL_DETAILED_EXCEPTION(RangeException)
	
	DECL_DETAILED_EXCEPTION(SQLException)
	
	struct DJBHash
	{
		size_t operator()(const string_t& hashKey) const;
	};
	
	/// ClassInfo<> implementation
	template<class T>
	std::unique_ptr<ClassInfo<T>> ClassInfo<T>::m_classInstance = nullptr;
	
	template<class T> size_t ClassInfo<T>::s_size = 0;
	
	template<class T>
	ClassInfo<T>::ClassInfo()
			: m_className {typeid(T).name()}
	{
		if (!std::is_null_pointer<T>::value)
			s_size = sizeof(T);
		else
			s_size = 0;
	}
	
	template<class T>
	ClassInfo<T>::~ClassInfo()
	{}
	
	template<class T>
	inline std::unique_ptr<ClassInfo<T>>&
	ClassInfo<T>::getInstance()
	{
		if (m_classInstance == nullptr)
			m_classInstance = std::unique_ptr<ClassInfo>(new ClassInfo());
		
		return m_classInstance;
	}
	
	template<class T>
	inline string_t ClassInfo<T>::getName()
	{
		return ClassNameConvertor::getName(getRawName());
	}
	
	template<class T>
	inline const char* ClassInfo<T>::getRawName()
	{
		return getInstance()->m_className;
	}
	
	template<class T>
	template<class Other>
	inline constexpr bool ClassInfo<T>::compare()
	{
		#ifdef HAVE_CXX17
		return std::is_same_v<T, Other>;
		#else
		return std::is_same<T, Other>::value;
		#endif
	}
	
	template<class T>
	inline constexpr i32 ClassInfo<T>::compareTo(const ClassInfo<T>& other)
	{
		if (getSize() - other.getSize() < 0)
			return -1;
		else if (getSize() - other.getSize() > 0)
			return 1;
		else
			return 0;
	}
	
	template<class T>
	inline const size_t& ClassInfo<T>::getSize()
	{
		return getInstance()->s_size;
	}
	
	template<typename... Args>
	inline void VarArgMessageFormat<Args...>::format(string_t& message, Args&&... args)
	{
		auto __pFmt = new Format(message);
		std::initializer_list<any_t> __argumentsAny {std::forward<Args>(args)..., sizeof...(Args)};
		
		for (auto& __anyArg : __argumentsAny)
		{
			append<string_t>(__pFmt, __anyArg);
			append<ustring_t>(__pFmt, __anyArg);
			append<const char*>(__pFmt, __anyArg);
			append<int>(__pFmt, __anyArg);
			append<double>(__pFmt, __anyArg);
			append<unsigned int>(__pFmt, __anyArg);
		}
		message.clear();
		message = __pFmt->str();
		SAFE_RELEASE(__pFmt)
	}
	
	template<typename... Args>
	template<typename T>
	inline constexpr bool VarArgMessageFormat<Args...>::checkTypeIdentity(const any_t& anyArg)
	{
		return typeid(T) == anyArg.type();
	}
	
	template<typename... Args>
	template<typename T>
	inline void VarArgMessageFormat<Args...>::append(Format* fmt, const any_t& arg)
	{
		if (checkTypeIdentity<T>(arg))
		{
			(*fmt) % Internal::any_cast<T>(arg);
		}
		return;
	}
} // namespace Dixter