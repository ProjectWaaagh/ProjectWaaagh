#include "Errors.hpp"
#include "StringFormat.hpp"

#include <cstdio>
#include <cstdlib>
#include <thread>
#include <cstdarg>

#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
#	include <Windows.h>
#	define Crash(message)	\
		ULONG_PTR exceptionArgs[] = { reinterpret_cast<ULONG_PTR>(strdup(message)), reinterpret_cast<ULONG_PTR>(_ReturnAddress()) }; \
		RaiseException(EXCEPTION_ASSERTION_FAILURE, 0, 2, exceptionArgs)
#else
	extern "C" { PRW_FRAMEWORK_API char const* ProjectWaaaghAssertionFailedMessage = nullptr; }
	#define Crash(message) \
		ProjectWaaaghAssertionFailedMessage = strdup(message); \
		*((volatile int*)nullptr) = 0; \
		exit(1);
#endif

namespace {
	std::string FormatAssertionMessage(char const* format, va_list args) {
		std::string formatted;
		va_list len;

		va_copy(len, args);
		int32 length = vsnprintf(nullptr, 0, format, len);
		va_end(len);

		formatted.resize(length);
		vsnprintf(&formatted[0], length + 1, format, args);

		return formatted;
	}
}

namespace ProjectWaaagh {

	void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message) {
	
		std::string formattedMessage =
			StringFormat("\n%s:%i in %s ASSERTION FAILED:\n	%s\n", file, line, function, message) + debugInfo + '\n';
		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);
		Crash(formattedMessage.c_str());
	}
	void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message, char const* format, ...) {
	
		va_list args;
		va_start(args, format);

		std::string formattedMessage =
			StringFormat("\n%s:%i in %s ASSERTION FAILED:\n	%s\n", file, line, function, message) + FormatAssertionMessage(format, args) + '\n' + debugInfo + '\n';
		va_end(args);

		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		Crash(formattedMessage.c_str());
	}
	
	void Fatal(char const* file, int line, char const* function, char const* message, ...) {

		va_list args;
		va_start(args, message);

		std::string formattedMessage =
			StringFormat("\n%s:%i in %s FATAL ERROR:\n", file, line, function) + FormatAssertionMessage(message, args) + '\n';
		va_end(args);

		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		std::this_thread::sleep_for(std::chrono::seconds(10));
		Crash(formattedMessage.c_str());
	}
	
	void Error(char const* file, int line, char const* function, char const* message) {

		std::string formattedMessage =
			StringFormat("\n%s:%i in %s ERROR:\n	%s\n", file, line, function, message);
		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		Crash(formattedMessage.c_str());
	}
	
	void Warning(char const*file, int line, char const* function, char const* message) {

		fprintf(stderr, "\n%s:%i in %s WARNING:\n	%s\n", file, line, function, message);
	}
	
	void Abort(char const* file, int line, char const* function) {

		std::string formattedMessage =
			StringFormat("\n%s:%i in %s ABORTED.\n", file, line, function);
		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		Crash(formattedMessage.c_str());
	}
	void Abort(char const* file, int line, char const* function, char const* message, ...) {

		va_list args;
		va_start(args, message);

		std::string formattedMessage =
			StringFormat("\n%s:%i in %s ABORTED:\n", file, line, function) + FormatAssertionMessage(message, args) + '\n';
		va_end(args);

		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		Crash(formattedMessage.c_str());
	}

	void AbortHandler(int sigVal) {
	
		std::string formattedMessage =
			StringFormat("Caught signal %i\n", sigVal);
		fprintf(stderr, "%s", formattedMessage.c_str());
		fflush(stderr);

		Crash(formattedMessage.c_str());
	}
}

std::string GetDebugInfo() {
	return "";
}