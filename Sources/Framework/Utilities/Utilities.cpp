#include "Utilities.hpp"
#include "Framework.hpp"

#include <boost/core/demangle.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdarg>
#include <ctime>

#if PROJECTWAAAGH_COMPILER == PROJECTWAAAGH_COMPILER_GNU
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#endif

std::vector<std::string_view> ProjectWaaagh::Tokenize(std::string_view stringToTokenize, char separator, bool keepEmpty) {

    std::vector<std::string_view> tokens;

    size_t start = 0;
    for(size_t end = stringToTokenize.find(separator); end != std::string_view::npos; end = stringToTokenize.find(separator, start)) {
        if(keepEmpty || (start < end))
            tokens.push_back(stringToTokenize.substr(start, end - start));
        start = end + 1;
    }

    if(keepEmpty || (start < stringToTokenize.length()))
        tokens.push_back(stringToTokenize.substr(start));

    return tokens;
}


bool StringEqualI(std::string_view str1, std::string_view str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char c1, char c2) {
        return std::tolower(c1) == std::tolower(c2);
        });
}