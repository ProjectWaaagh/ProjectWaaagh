#ifndef PROJECTWAAAGH_UTILITIES_HPP
#define PROJECTWAAAGH_UTILITIES_HPP

#include "Defines.hpp"
#include "Errors.hpp"

#include <array>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>


namespace ProjectWaaagh {
    PRW_FRAMEWORK_API std::vector<std::string_view> Tokenize(std::string_view stringToTokenize, char separator, bool keepEmpty);

    std::vector<std::string_view> Tokenize(std::string&&, char, bool) = delete;
    std::vector<std::string_view> Tokenize(std::string const&&, char, bool) = delete;

    inline std::vector<std::string_view> Tokenize(char const* stringToTokenize, char separator, bool keepEmpty) {
        return Tokenize(std::string_view(stringToTokenize ? stringToTokenize : ""), separator, keepEmpty);
    }
}


PRW_FRAMEWORK_API bool StringEqualI(std::string_view str1, std::string_view str2);

#endif //PROJECTWAAAGH_UTILITIES_HPP
