/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */

#pragma once

#include "setup.h"

#ifndef WIN32
#	ifdef OTR_EXPORTS
#		define DIXTER_API __declspec(dllexport)
#	else
#		define DIXTER_API __declspec(dllimport)
#	endif
#else
	# define DIXTER_API
#endif //API endif

#define dxDEPRECATED            [[deprecated]]
#define dxDEPRECATEDR(reason)   [[deprecated(xSTR(reason))]]
#define dxVISIBILITY(v)         __attribute__((visibility(v)))
#define dxHIDDEN                dxVISIBILITY("hidden")
#define dxMAYBE_UNUSED          [[maybe_unused]]
#define dxNORETURN              [[noreturn]]

#ifndef dxSTR
#	define dxSTR(str) #str
#endif //dxSTR

#ifndef override
#	define override override
#endif

#ifdef CUDA_ENABLED
#	include <cuda_runtime.h>
#	define cudev __device__
#	define cuhost __host__
#	define cuglob __global__
#	define cuconst __constant__
#	define cuhostdev cuhost cudev
#else
#	define cudev
#	define cuhost
#	define cuglob
#	define cuconst
#endif

#ifdef _ASSERT_H
#   define dxASSERT(expr) assert(expr);
#   define dxASSERT_MSG(expr, message) assert((expr)&&(message));
#endif

#ifdef HAVE_CXX17
#define dxMAYBE_UNUSED 	[[maybe_unused]]
#define dxNORETURN 		[[noreturn]]
#else
#define dxMAYBE_UNUSED
#define dxNORETURN
#endif

#ifndef dxUNUSED
#define dxUNUSED(x) (void)x;
#endif

#ifndef WSTR
#define WSTR(str) L(str.c_str())
#endif //WSTR

#define prints(str)                     std::cout<<str;
#define printeol                        prints('\n');
#define printfm(str, ...)               printf(str, __VA_ARGS__);
#define print_log0(str)                 std::clog<<"Log "<<__TIME__<<", "<<						\
                                        __DATE__<<": "<<__FUNCTION__<<"() "<<str
#define print_log(str)                  print_log0(str);
#define printclog(str)                  std::clog << str;
#define printl_log(str)                 print_log0(str)<<std::endl;
#define printw(str)                     std::wcout<<str;
#define printc(str)                     std::cout<<str.c_str();

#ifndef println
#define println(str)                    std::cout<<str<<std::endl;
#define printlw(str)                    std::wcout<<str<<std::endl;
#define printlc(str)                    println(str.c_str())
#define printn(Object)                  prints(#Object)
#define printlv(value)                  prints(xSTR(value:)) print(" ") println(value)
#define printlvu(value)                 prints(xSTR(value:) <<" ") printlc(value)
#define printv(value)                   prints(xSTR(value:) <<" ") prints(value<<" ")
#define printb(booleanExpr)             println((booleanExpr ? "True" : "False"))
#define printvar(var)                   prints("\t"<<xSTR(var))
#define printvarl(var)                  println("\t"<<xSTR(var))
#define printbe(str, booleanExpr, \
                expr_true, expr_false)  println(str<<" "<<(booleanExpr?expr_true:expr_false))
#define printbs(str, booleanExpr)       printbe(str, booleanExpr, "True", "False");
#define dxEXCEPTION_DETAIL              __DATE__<<' '<<__TIME__<<' '<<__FILE__<<':'<<__LINE__
#define printerr_detail(error_msg, \
                        detail)         std::cerr<<detail<<" "<<error_msg<<std::endl;
#define printerr(msg)                    printerr_detail(msg, dxEXCEPTION_DETAIL)
#define snprintfm(msg, format, ...)                                                             \
                                        std::unique_ptr<TByte[]> msg(new TByte[255]);           \
                                        snprintf(msg.get(), 255, format, __VA_ARGS__);          \


#endif

#ifndef ENUM_CAST
#define ENUM_CAST(Type, object) static_cast<typename std::underlying_type<Type>::type>(object)
#endif

#if defined(HAVE_CXX14) or defined(HAVE_CXX17)
#   ifndef dxMAKE_UNIQUE
#       define dxMAKE_UNIQUE(Type, ...) std::make_unique<Type>(__VA_ARGS__)
#   endif
#   ifndef dxMAKE_SHARED
#       define dxMAKE_SHARED(Type, ...) std::make_shared<Type>(__VA_ARGS__)
#   endif
#else
#   ifndef dxMAKE_UNIQUE
#       define dxMAKE_UNIQUE(Type, ...) std::unique_ptr<Type>(new Type(__VA_ARGS__))
#   endif
#   ifndef dxMAKE_SHARED
#       define dxMAKE_SHARED(Type, ...) std::shared_ptr<Type>(new Type(__VA_ARGS__))
#   endif
#endif