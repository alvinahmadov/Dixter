/**
 *  Copyright (C) 2015-2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 *
 *  This file is part of Dixter Project
 *  License-Identifier: MIT License
 *  See README.md for more information.
 */
#pragma once

#ifndef WIN32
#include <bits/c++config.h>
#endif

#ifndef APPNAME
#define APPNAME Dixter
#endif

#define DIXTER_VERSION_MAJOR 1
#define DIXTER_VERSION_MINOR 0
#define DIXTER_VERION_RELEASE 0

#define DIXTER_VERSION DIXTER_VERSION_MAJOR.DIXTER_VERSION_MINOR.VERSION_RELEASE

#ifdef __cplusplus
#   if     __cplusplus >= 201703L
#        define HAVE_CXX17
#        define HAVE_CXX14
#        define HAVE_CXX11
#   elif   __cplusplus >= 201402L and __cplusplus < 201703L
#        define HAVE_CXX14
#        define HAVE_CXX11
#   elif __cplusplus == 201103L
#        define HAVE_CXX11
#   else
#        error("Dixter requires modern C++")
#   endif
#endif

#if __has_include(<QString>)
#define HAS_QSTRING
#endif

#undef DIXTER_DEBUG
#undef USE_ESPEAK
#define USE_LOGGING
#define SSYNTH_DEBUG 0

#if defined(__GLIBCXX__) || defined(__linux__) || defined(linux)
#   ifndef UNIX
#       define UNIX
#   endif
#elif defined(_WIN32)
#   ifndef WIN32
#       define WIN32
#   endif
#endif

#ifndef HAS_WCHAR
#   ifdef UNIX
#       ifdef _GLIBCXX_USE_WCHAR_T
#           define HAS_WCHAR
#       endif
#   elif defined(WIN32)
#       define HAS_WCHAR
#   endif
#endif

#ifndef USE_C99_STDINT_TR1
#   ifdef UNIX
#       ifdef _GLIBCXX_USE_C99_STDINT_TR1
#           define USE_C99_STDINT_TR1
#       endif
#   elif defined(WIN32)
#       define USE_WCHART16
#       define USE_WCHART32
#   endif
#endif

#ifdef USE_C99_STDINT_TR1
#define USE_WCHART16
#define USE_WCHART32
#endif

#ifdef USE_TO_STRING_CONVERSION
#undef USE_TO_STRING_CONVERSION
#endif