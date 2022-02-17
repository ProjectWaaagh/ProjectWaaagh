#ifndef PROJECTWAAAGH_ERRORS_HPP
#define PROJECTWAAAGH_ERRORS_HPP

#include "Defines.hpp"

#include <string>

namespace ProjectWaaagh {
	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message) ATTR_NORETURN;
	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message, char const* format, ...) ATTR_NORETURN ATTR_PRINTF(6, 7);

	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Fatal(char const* file, int line, char const* function, char const* message, ...) ATTR_NORETURN ATTR_PRINTF(4, 5);

	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Error(char const* file, int line, char const* function, char const* message) ATTR_NORETURN;

	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Abort(char const* file, int line, char const* function) ATTR_NORETURN;
	DECLSPEC_NORETURN PRW_FRAMEWORK_API void Abort(char const* file, int line, char const* function, char const* message, ...) ATTR_NORETURN;

	PRW_FRAMEWORK_API void Warning(char const* file, int line, char const* function, char const* message);

	DECLSPEC_NORETURN PRW_FRAMEWORK_API void AbortHandler(int sigVal) ATTR_NORETURN;
}

PRW_FRAMEWORK_API std::string GetDebugInfo();

#if PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_MICROSOFT
#	define ASSERT_BEGIN __pragma(warning(push)) __pragma(warning(disable: 4127))
#	define ASSERT_END __pragma(warning(pop))
#else
#	define ASSERT_BEGIN
#	define ASSERT_END
#endif

#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
#	define EXCEPTION_ASSERTION_FAILURE 0xC0000420L
#endif

#define WPAssert(cond, ...) ASSERT_BEGIN do { if (!(cond)) ProjectWaaagh::Assert(__FILE__, __LINE__, __FUNCTION__, GetDebugInfo(), #cond, ##__VA_ARGS__); } while(0) ASSERT_END
#define WPAssert_NODEBUGINFO(cond, ...) ASSERT_BEGIN do { if (!(cond)) ProjectWaaagh::Assert(__FILE__, __LINE__, __FUNCTION__, "", #cond, ##__VA_ARGS__); } while(0) ASSERT_END
#define WPFatal(cond, ...) ASSERT_BEGIN do { if (!(cond)) ProjectWaaagh::Fatal(__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); } while(0) ASSERT_END
#define WPError(cond, msg) ASSERT_BEGIN do { if (!(cond)) ProjectWaaagh::Error(__FILE__, __LINE__, __FUNCTION__, (msg)); } while(0) ASSERT_END
#define WPWarning(cond, msg) ASSERT_BEGIN do { if (!(cond)) ProjectWaaagh::Warning(__FILE__, __LINE__, __FUNCTION__, (msg)); } while(0) ASSERT_END
#define WPAbort() ASSERT_BEGIN do { ProjectWaaagh::Abort(__FILE__, __LINE__, __FUNCTION__); } while(0) ASSERT_END
#define WPAbort_MSG(msg, ...) ASSERT_BEGIN do { ProjectWaaagh::Abort(__FILE__, __LINE__, __FUNCTION__, (msg), ##__VA_ARGS__); } while(0) ASSERT_END


#ifdef PERFORMANCE_PROFILING
#	define ASSERT(cond, ...) ((void)0)
#	define ASSERT_NODEBUGINFO(cond, ...) ((void)0)
#else
#	define ASSERT WPAssert
#	define ASSERT_NODEBUGINFO WPAssert_NODEBUGINFO
#endif // PERFORMANCE_PROFILING

#define ASSERT_WITH_SIDE_EFFECTS WPAssert

#define ABORT WPAbort
#define ABORT_MSG WPAbort_MSG

template<typename T>
inline T* ASSERT_NOTNULL_IMPL(T* pointer, char const* expr) {
	ASSERT(pointer, "%s", expr);
	return pointer;
}

#define ASSSERT_NOTNULL(pointer) ASSERT_NOTNULL_IMPL(pointer, #pointer)



#endif //PROJECTWAAAGH_ERRORS_HPP
