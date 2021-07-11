#ifndef PROJECT_WAAAGH_DEFINES_HPP
#define PROJECT_WAAAGH_DEFINES_HPP


#include "CompilerDefines.hpp"

#if PROJECT_WAAAGH_COMPLER == PROJECT_WAAAGH_COMPLER_GNU
#   if !defined(__STDC_FORMAT_MACROS)
#       define __STDC_FORMAT_MACROS
#   endif
#   if !defined(__STDC_CONSTANT_MACROS)
#       define __STDC_CONSTANT_MACROS
#   endif
#   if !defined(_GLIBCXX_USE_NANOSLEEP)
#       define _GLIBCXX_USE_NANOSLEEP
#   endif
#   if defined(HELGRIND)
#       include <valgrind/helgrind.h>
#       undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#       undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#       define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A)
#       define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)
#   endif
#   if defined(VALGRIND)
#       include <valgrind/memcheck.h>
#   endif
#endif


#include <cstddef>
#include <cinttypes>
#include <climits>

#if PROJECT_WAAAGH_PLATFORM == PROJECT_WAAAGH_PLATFORM_WINDOWS
#   define PROJECT_WAAAGH_PATH_MAX 260
#   define _USE_MATH_DEFINES
#   ifndef DECLSPEC_NORETURN
#       define DECLSPEC_NORETURN __declspec(noreturn)
#   endif
#   ifndef DECLSPEC_DEPRECATED
#       define DECLSPEC_DEPRECATED __declspec(deprecated)
#   endif
#else //PROJECT_WAAAGH_PLATFORM != PROJECT_WAAAGH_PLATFORM_WINDOWS
#   define PROJECT_WAAAGH_PATH_MAX PATH_MAX
#   define DECLSPEC_NORETURN
#   define DECLSPEC_DEPRECATED
#endif


#ifdef PROJECT_WAAAGH_API_EXPORT_FRAMEWORK
#   define PW_FRAMEWORK_API PW_API_EXPORT
#else
#   define PW_FRAMEWORK_API PW_API_IMPORT
#endif

#ifdef PROJECT_WAAAGH_API_EXPORT_DATABASE
#   define PW_DATABASE_API PW_API_EXPORT
#else
#   define PW_DATABASE_API PW_API_IMPORT
#endif

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;


#endif //PROJECT_WAAAGH_DEFINES_HPP