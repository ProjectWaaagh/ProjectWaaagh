#ifndef PROJECT_WAAAGH_FRAMEWORK_STRINGFORMAT_HPP
#define PROJECT_WAAAGH_FRAMEWORK_STRINGFORMAT_HPP

#include <fmt/core.h>

namespace ProjectWaaagh {

    /**
     * @brief 
     * 
     * @tparam Format 
     * @tparam Args 
     * @param format 
     * @param args 
     * @return std::string 
     */
    template<typename Format, typename... Args>
    inline std::string StringFormat(Format&& format, Args&&... args) {
        return fmt::print(std::forward<Format>(format), std::forward<Args>(args)...);
    }

    inline bool FormatIsEmptyOrNull(char const* format) {
        return format == nullptr;
    }

    inline bool FormatIsEmptyOrNull(std::string const& format) {
        return format.empty();
    }
}

#endif //PROJECT_WAAAGH_FRAMEWORK_STRINGFORMAT_HPP