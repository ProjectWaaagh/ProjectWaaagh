#ifndef PROJECT_WAAAGH_COMPILER_DEFINES_HPP
#define PROJECT_WAAAGH_COMPILER_DEFINES_HPP


#define PROJECT_WAAAGH_PLATFORM_WINDOWS     0
#define PROJECT_WAAAGH_PLATFORM_UNIX        1
#define PROJECT_WAAAGH_PLATFORM_APPLE       2
#define PROJECT_WAAAGH_PLATFORM_INTEL       3

#define PROJECT_WAAAGH_COMPILER_MICROSOFT   0
#define PROJECT_WAAAGH_COMPILER_GNU         1
#define PROJECT_WAAAGH_COMPILER_BORLAND     2
#define PROJECT_WAAAGH_COMPILER_INTEL       3

#if defined(_WIN64)
#   define PROJECT_WAAAGH_PLATFORM PROJECT_WAAAGH_PLATFORM_WINDOWS
#elif defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#   define PROJECT_WAAAGH_PLATFORM PROJECT_WAAAGH_PLATFORM_WINDOWS
#elif defined(__APPLE_CC__)
#   define PROJECT_WAAAGH_PLATFORM PROJECT_WAAAGH_PLATFORM_APPLE
#elif defined(__INTEL_COMPILER)
#   define PROJECT_WAAAGH_PLATFORM PROJECT_WAAAGH_PLATFORM_INTEL
#else
#   define PROJECT_WAAAGH_PLATFORM PROJECT_WAAAGH_PLATFORM_UNIX
#endif

#ifdef _MSC_VER
#   define PROJECT_WAAAGH_COMPILER PROJECT_WAAAGH_COMPILER_MICROSOFT
#elif defined(__BORLANDC__)
#   define PROJECT_WAAAGH_COMPILER PROJECT_WAAAGH_COMPILER_BORLAND
#elif defined(__INTEL_COMPILER)
#   define PROJECT_WAAAGH_COMPILER PROJECT_WAAAGH_COMPILER_INTEL
#elif defined(__GNUC__)
#   define PROJECT_WAAAGH_COMPILER PROJECT_WAAAGH_COMPILER_GNU
#   define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif
#   error "FATAL ERROR: UNKNOWN COMPILER!"
#endif

#endif //PROJECT_WAAAGH_COMPILER_DEFINES_HPP