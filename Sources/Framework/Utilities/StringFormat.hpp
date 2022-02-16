#ifndef PROJECTWAAAGH_STRINGFORMAT_HPP
#define PROJECTWAAAGH_STRINGFORMAT_HPP

#include "fmt/printf.h"

namespace ProjectWaaagh {

	template<typename Format, typename... Args>
	inline std::string StringFormat(Format&& fmt, Args&&... args) {
		try {
			return fmt::sprintf(std::forward<Format>(fmt), std::forward<Args>(args)...);
		}
		catch (const fmt::format_error& formatError) {
			std::string error = "An error occurred formatting string \"" + std::string(fmt) + "\" : " + std::string(formatError.what());
			return error;
		}
	}

	inline bool isFormatEmptyOrNull(char const* fmt) {
		return fmt == nullptr;
	}

	inline bool IsFormatEmptyOrNull(std::string const& fmt) {
		return fmt.empty();
	}
}

#endif // !PROJECTWAAAGH_STRINGFORMAT_HPP
