/**
 * @author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 * @date   28/03/15
 */

#ifndef _DIXTER_MACROS_H_
#define _DIXTER_MACROS_H_


#include <iostream>
#include "setup.h"


#ifndef WIN32
# ifdef OTR_EXPORTS
#  define DIXTER_API __declspec(dllexport)
# else
#  define DIXTER_API __declspec(dllimport)
# endif
#else
# define DIXTER_API
#endif //API endif

#ifdef HAVE_CXX11
#define dxDEPRECATED _GLIBCXX_DEPRECATED
#define dxDEPRECATEDR(reason) [[deprecated(xSTR(reason))]]
#define dxVISIBILITY(v) __attribute__((visibility(v)))
#endif

#define DX_INTERNAL_NOEXPORT dxVISIBILITY("hidden")

#ifndef xSTR
#define xSTR(str) #str
#endif //xSTR

#if defined(HAVE_CXX11)
#ifndef dxDECL_OVERRIDE
#define dxDECL_OVERRIDE override
#endif

#ifndef dxDECL_NOEXCEPT
#define dxDECL_NOEXCEPT noexcept
#endif

#ifndef dxDECL_CONSTEXPR
#define dxDECL_CONSTEXPR constexpr
#endif
#elif defined(CXX_LEGACY)
#ifndef dxDECL_OVERRIDE
#define dxDECL_OVERRIDE
#endif

#ifndef dxNOEXCEPT
#define dxNOEXCEPT
#endif

#ifndef dxCONSTEXPR
#define dxCONSTEXPR const
#endif
#endif //HAVE_CXX

#ifdef CUDA_ENABLED
#include <crt/host_defines.h>
#define cudev __device__
#define cuhost __host__
#define cuglob __global__
#define cuconst __constant__
#define cuhostdev cuhost cudev
#else
#define cudev
#define cuhost
#define cuglob
#define cuconst
#endif

#ifdef _ASSERT_H
#   define dxASSERT(expr) assert(expr);
#   define dxASSERT_MSG(expr, message) assert((expr)&&(message));
#endif

#define dxEXCEPTION_DETAIL __FILE__<<':'<<__LINE__

#ifndef WSTR
#define WSTR(str) L(str.c_str())
#endif //WSTR

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pointer)                                                       \
if(pointer != nullptr)                                                              \
{                                                                                   \
    delete pointer;                                                                 \
    pointer = nullptr;                                                              \
}

#define SAFE_RELEASE_ARRAY(array)                                                   \
if(array)                                                                           \
{                                                                                   \
    delete[] array;                                                                 \
    array = nullptr;                                                                \
};

#define SAFE_RELEASE_LIST(listPtr)                                                  \
if (listPtr.size() > 0)                                                             \
{                                                                                   \
    for (auto& item : (listPtr))                                                    \
    {                                                                               \
        SAFE_RELEASE(item);                                                         \
    }                                                                               \
};

#define SAFE_RELEASE_MAP(map)                                                       \
if (map.size() > 0)                                                                 \
{                                                                                   \
    for (auto& value : (map))                                                       \
    {                                                                               \
        SAFE_RELEASE(value.second);                                                 \
    }                                                                               \
};

#ifdef HAVE_CXX11
#define dxRESET(smart_ptr)                                                          \
if (smart_ptr.get() != nullptr)                                                     \
{                                                                                   \
    smart_ptr.reset();                                                              \
}

#define SAFE_RESET_LIST(list)                                                       \
if (list.size() > 0)                                                                \
{                                                                                   \
    for (auto& lst : list)                                                          \
    {                                                                               \
        lst.reset();                                                                \
    }                                                                               \
}

#define SAFE_RESET_MAP(map)                                                         \
if (map.size() > 0)                                                                 \
{                                                                                   \
    for (auto& pr : map)                                                            \
    {                                                                               \
        pr.second.reset();                                                          \
    }                                                                               \
}

#elif defined(CXX_LEGACY)
#define dxRESET(smart_ptr) SAFE_RELEASE(smart_ptr)
#endif
#endif //SAFE_RELEASE, dxRESET

#ifndef print
#define prints(str) std::cout<<str;
#define print_log0(str) std::cout<< "Log " << __TIME__ << ", " << __DATE__ << ": "<< __FUNCTION__ << "() " << str
#define print_log(str) print_log0(str);
#define printl_log(str) print_log0(str) << std::endl;
#define printw(str) std::wcout<<str;
#define printc(str) std::cout<<str.c_str();
#ifndef printl
#define printl(str) std::cout<<str<<std::endl;
#define prints_debug(str, args...) auto __strng = std::string(str); __strng.insert(0, "%s:%d Debugging: "); std::printf(__strng.c_str(), __FILE__, __LINE__, args);
#define printl_debug(str, args...) prints_debug(str, args) printl("")

#define printf_debug(str, ...) std::cout<<str<<...;
#define printlw(str) std::wcout<<str<<std::endl;
#define printlc(str) PUTL(str.c_str())
#define printf_(str) if (!str.empty()) {PUT(str)}
#define printlf(str) if (!str.empty()) {PUTL(str)}
#define printn(Object) print(#Object)
#define printlv(value)     print(xSTR(value:)) print(" ") printl(value)
#define printlvu(value)    print(xSTR(value:)) P_ printlc(value)
#define printv(value)      print(xSTR(value:)) print(" ") print(value) print(" ")
#define printb(booleanExpr) printl((booleanExpr ? "True" : "False"))
#define printvar(var) print("\t") print(xSTR(var))
#define printvarl(var) print("\t") printl(xSTR(var))
#define printbe(str, booleanExpr, expr_true, expr_false) std::cout<< str << " " << (booleanExpr ? expr_true : expr_false) << std::endl;
#define printbs(str, booleanExpr) printbe(str, booleanExpr, "True", "False");
#define printerr_detail(error_msg, detail) std::cerr<<detail<<' '<<error_msg<<std::endl;
#define printerr(error_msg) printerr_detail(error_msg, dxEXCEPTION_DETAIL)
#endif

#ifndef EOL
#define EOL std::cout<<std::endl;
#endif

#ifndef DYN_CAST
#define DYN_CAST(Type, object) dynamic_cast<Type>(object)
#endif

#ifndef STAT_CAST
#define STAT_CAST(Type, object) static_cast<Type>(object)
#endif

#ifndef ENUM_CAST
#define ENUM_CAST(Type, object) static_cast<typename std::underlying_type<Type>::type>(object)
#endif

#ifndef CERR
#define CERR(err_msg) std::cerr << err_msg << std::endl;
#endif
#endif

#ifndef TRY
#define TRY try {

#define CATCH_PARAM(exc) (exc& e)
#define CATCH_BODY { CERR(e.what()); }
#define CATCH(exc) } catch CATCH_PARAM(exc) CATCH_BODY
#define CATCH_DEFAULT CATCH_PARAM CATCH_BODY

#define CATCH_PARAM_IGNORE (...)
#define CATCH_BODY_IGNORE { /* CERR(e.what()) */ }
#define CATCH_IGNORE } catch CATCH_PARAM_IGNORE CATCH_BODY_IGNORE
#define CATCH_DEFAULT_IGNORE CATCH_IGNORE

#ifndef TRY_DEFAULT
#define TRY_DEFAULT(expr) TRY expr; CATCH(std::exception)
#endif

#endif

#ifndef ENABLE_THEME
#if NOTHEME
#undef ENABLE_THEME
#else
#define ENABLE_THEME
#endif
#endif

#ifdef ENABLE_THEME
#define dxSET_COLOUR_THEME(conf_member, configRoot, backgColour, foregColour)       \
SetBackgroundColour(wxColour(conf_member->getAccessor()                             \
                                        ->getValue(configRoot, backgColour).asCustom()));      \
SetForegroundColour(wxColour(conf_member->getAccessor()                             \
                                        ->getValue(configRoot, foregColour).asCustom()));      \

#define dxSET_FONT_THEME(conf_member, configRoot, fontNameNode, fontSizeNode)       \
SetFont(wxFont(wxFontInfo(US2I(conf_member->getAccessor()                           \
                                            ->getValue(configRoot, fontSizeNode)))  \
                       .FaceName(conf_member->getAccessor()                         \
                                            ->getValue(configRoot, fontNameNode).asCustom())   \
                       .AntiAliased(true)));
#else
#define dxSET_COLOUR_THEME(conf_member, backgColour, foregColour, rootNode)
#define dxSET_FONT_THEME(conf_member, fontNameNode, fontSizeNode, rootNode)
#endif

#ifndef MKUNIQ
#define MKUNIQ(Type, x) std::make_unique<Type>(x)
#endif

#ifndef MKSHARD
#define MKSHARD(Type, x) std::make_shared<Type>(x)
#endif

#ifndef MKPAIR
#define MKPAIR(Type1, Type2, x1, x2) std::make_pair<Type1, Type2>(x1, x2)
#endif

#ifndef IGNORE
#define IGNORE
#endif

#ifndef US2I
#define US2I(str) static_cast<int>(std::strtol(str.c_str(), NULL, 0))
#endif
#ifndef S2I
#define S2I(str)  std::strtol(str.c_str(), NULL, 0)
#endif

#ifndef BIND_EVENT
#define BIND_EVENT(EvtType, method) this->Bind(EvtType, method, this);
#endif

#ifndef BIND_EVENT_A
#define BIND_EVENT_A(obj, EvtType, method)  obj->Bind(EvtType, method, this);
#endif

#ifndef BIND_EVENT_B
#define BIND_EVENT_B(EvtType, Class, method, EnumType, enumParam)                  \
Bind(EvtType, &Class::method, this, (unsigned int) EnumType::enumParam);
#endif

#ifndef BIND_EVENT_C
#define BIND_EVENT_C(obj, EvtType, Class, method, EnumType, enumParam)             \
obj->Bind(EvtType, &Class::method, (unsigned int) EnumType::enumParam);
#endif

#ifndef BIND_EVENT_D
#define BIND_EVENT_D(EvtType, Class, method)                                       \
BIND_EVENT(EvtType, (&Class::method));
#endif

#endif //_DIXTER_MACROS_H_