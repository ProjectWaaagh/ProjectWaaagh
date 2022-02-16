#ifndef PROJECTWAAAGH_DEFINES_HPP
#define PROJECTWAAAGH_DEFINES_HPP

#include "CompilerDefs.hpp"

#if PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_GNU
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
#       define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) ANNOTATE_HAPPENS_BEFORE(A)
#       define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)  ANNOTATE_HAPPENS_AFTER(A)
#   endif
#   if defined(VALGRIND)
#       include <valgrind/memcheck.h>
#   endif
#endif//PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_GNU


#include <cstddef>
#include <cinttypes>
#include <climits>

#define PROJECTWAAAGH_LITTLEENDIAN  0
#define PROJECTWAAAGH_BIGENDIAN     1

#if !defined(PROJECTWAAAGH_ENDIAN)
#   if defined(BOOST_BIG_ENDIAN)
#       define PROJECTWAAAGH_ENDIAN PROJECTWAAAGH_BIGENDIAN
#   else
#       define PROJECTWAAAGH_ENDIAN PROJECTWAAAGH_LITTLEENDIAN
#   endif
#endif

#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
#   define PROJECTWAAAGH_PATH_MAX 260
#   define _USE_MATH_DEFINES
#   ifndef DECLSPEC_NORETURN
#       define DECLSPEC_NORETURN __declspec(noreturn)
#   endif
#   ifndef DECLSPEC_DEPRECATED
#       define DECLSPEC_DEPRECATED __declspec(deprecated)
#   endif
#else // PROJECTWAAAGH_PLATFORM != PROJECTWAAAGH_PLATFORM_WINDOWS
#   define PROJECTWAAAGH_PATH_MAX PATH_MAX
#   define DECLSPEC_NORETURN
#   define DECLSPEC_DEPRECATED
#endif //PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS

#if !defined(COREDEBUG)
#   define PROJECTWAAAGH_INLINE inline
#else
#   if !defined(PROJECTWAAAGH_DEBUG)
#   define PROJECTWAAAGH_DEBUG
#   endif
#   define PROJECTWAAAGH_INLINE
#endif

#if PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_GNU
#   define ATTR_NORETURN __attribute__((__noreturn__))
#   define ATTR_PRINTF(F, V) __attribute((__format__ (__printf__, F, V)))
#   define ATTR_DEPRECATED __attribute__((__deprecated__))
#else
#   define ATTR_NORETURN
#   define ATTR_PRINTF(F, V)
#   define ATTR_DEPRECATED
#endif

#ifdef PROJECTWAAAGH_API_USE_DYNAMIC_LINKING
#   if PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_MICROSOFT
#       define PRW_API_EXPORT __declspec(dllexport)
#       define PRW_API_IMPORT __declspec(dllimport)
#   elif PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_GNU
#       define PRW_API_EXPORT __attribute__((visibility("default")))
#       define PRW_API_IMPORT
#   else
#       error Compiler not supported!
#   endif
#else
#   define PRW_API_EXPORT
#   define PRW_API_IMPORT
#endif

#ifdef PRW_API_EXPORT_FRAMEWORK
#   define PRW_FRAMEWORK_API PRW_API_EXPORT
#else
#   define PRW_FRAMEWORK_API PRW_API_IMPORT
#endif

#ifdef PRW_API_EXPORT_PROTO
#   define PRW_PROTO_API PRW_API_EXPORT
#else
#   define PRW_PROTO_API PRW_API_IMPORT
#endif

#ifdef PRW_API_EXPORT_DATABASE
#   define PRW_DATABASE_API PRW_API_EXPORT
#else
#   define PRW_DATABASE_API PRW_API_IMPORT
#endif

#ifdef PRW_API_EXPORT_SHARED
#   define PRW_SHARED_API PRW_API_EXPORT
#else
#   define PRW_SHARED_API PRW_API_IMPORT
#endif

#ifdef PRW_API_EXPORT_GAME
#   define PRW_GAME_API PRW_API_EXPORT
#else
#   define PRW_GAME_API PRW_API_IMPORT
#endif

#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

#define SZFMTD "%" PRIuPTR

#define STRING_VIEW_FMT "%.*s"
#define STRING_VIEW_FMT_ARG(str) static_cast<int>((str).length()), (str).data()

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

#endif //PROJECTWAAAGH_DEFINES_HPP