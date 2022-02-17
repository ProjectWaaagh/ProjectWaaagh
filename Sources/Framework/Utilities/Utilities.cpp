#include "Utilities.hpp"
#include "Framework.hpp"
#include "Containers.hpp"
#include "StringConverter.hpp"
#include "StringFormat.hpp"

#include <boost/core/demangle.hpp>
#include <utf8.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdarg>
#include <ctime>
#include <Windows.h>

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

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
struct tm* localtime_r(time_t const* time, struct tm *result) {
    localtime_s(result, time);
    return result;
}
#endif

bool StringEqualI(std::string_view str1, std::string_view str2) {
    return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), [](char c1, char c2) {
        return std::tolower(c1) == std::tolower(c2);
        });
}

size_t Utf8Length(std::string& Utf8Str) {
    try {
        return utf8::distance(Utf8Str.c_str(), Utf8Str.c_str() + Utf8Str.size());
    }
    catch (std::exception const&) {
        Utf8Str.clear();
        return 0;
    }
}
void Utf8Truncate(std::string& Utf8Str, size_t length) {
    try {
        size_t wLength = utf8::distance(Utf8Str.c_str(), Utf8Str.c_str() + Utf8Str.size());
        if (wLength <= length)
            return;

        std::wstring wString;
        wString.resize(wLength);
        utf8::utf8to16(Utf8Str.c_str(), Utf8Str.c_str() + Utf8Str.size(), &wString[0]);
        wString.resize(length);
        char* oend = utf8::utf8to16(wString.c_str(), wString.c_str() + wString.size(), &Utf8Str[0]);
        Utf8Str.resize(oend - (&Utf8Str[0]));
    }
    catch (std::exception const&) {
        Utf8Str.clear();
    }
}
bool Utf8ToWStr(char const* utf8Str, size_t cSize, wchar_t* WStr, size_t& WSize) {
    try {
        ProjectWaaagh::CheckedBufferOutputIterator<wchar_t> out(WStr, WSize);
        out = utf8::utf8to16(utf8Str, utf8Str+cSize, out);
        WSize -=out.remaining();
        WStr[WSize] = L'\0';
    } catch (std::exception const&) {
        wchar_t const* errorMessage = L"An error occurred converting string from UTF-8 to WStr";
        size_t errorMessageLength = wcslen(errorMessage);
        if(WSize >= errorMessageLength) {
            wcscpy(WStr, errorMessage);
            WSize = wcslen(WStr);
        } else if(WSize > 0) {
            WStr[0] = L'\0';
            WSize = 0;
        } else
            WSize = 0;

        return false;
    }

    return true;
}
bool Utf8ToWStr(std::string_view utf8Str, std::wstring& WStr) {
    WStr.clear();
    try {
        utf8::utf8to16(utf8Str.begin(), utf8Str.end(), std::back_inserter(WStr));
    } catch (std::exception const&) {
        WStr.clear();
        return false;
    }

    return true;
}

bool WStrToUtf8(std::wstring_view WStr, std::string& Utf8Str) {
    try {
        std::string utf8str2;
        utf8str2.resize(WStr.size()*4);

        if(WStr.size()) {
            char* oend = utf8::utf16to8(WStr.begin(), WStr.end(), &utf8str2[0]);
            utf8str2.resize(oend-(&utf8str2[0]));
        }

        Utf8Str = utf8str2;
    } catch (std::exception const&) {
        Utf8Str.clear();
        return false;
    }

    return true;
}
bool WStrToUtf8(wchar_t const* WStr, size_t Size, std::string& Utf8Str) {
    try {
        std::string utf8str2;
        utf8str2.resize(Size*4);

        if (Size) {
            char* oend = utf8::utf16to8(WStr, WStr+Size, &utf8str2[0]);
            utf8str2.resize(oend-(&utf8str2[0]));
        }
        Utf8Str = utf8str2;
    }
    catch (std::exception const&) {
        Utf8Str.clear();
        return false;
    }

    return true;
}

bool utf8ToConsole(std::string_view utf8str, std::string& conStr) {
#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
    std::wstring wstr;
    if(!Utf8ToWStr(utf8str, wstr))
        return false;

    conStr.resize(wstr.size());
    CharToOemBuffW(&wstr[0], &conStr[0], uint32(wstr.size()));
#else
    conStr = utf8str;
#endif

    return true;
}

bool consoleToUtf8(std::string_view conStr, std::string& utf8Str) {
#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
    std::wstring wstr;
    wstr.resize(conStr.size());
    OemToCharBuffW(&conStr[0], &wstr[0], uint32(conStr.size()));

    return WStrToUtf8(wstr, utf8Str);
#else
    utf8Str = conStr;
    return true;
#endif;
}
bool Utf8FitTo(std::string_view str, std::wstring_view search) {
    std::wstring temp;

    if(!Utf8ToWStr(str, temp))
        return false;

    wstringToLower(temp);

    if(temp.find(search) == std::wstring::npos)
        return false;

    return true;
}
void utf8printf(FILE* out, const char* str, ...) {
    va_list ap;
    va_start(ap, str);
    vutf8printf(out, str, &ap);
    va_end(ap);
}
void vutf8printf(FILE* out, const char* str, va_list* ap) {
#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
    char temp_buf[32 * 1024];
    wchar_t wtemp_buf[32 * 1024];

    size_t temp_len = vsnprintf(temp_buf, 32 * 1024, str, *ap);

    if(temp_len == size_t(-1))
        temp_len = 32*1024-1;

    size_t wtemp_len = 32*1024-1;
    Utf8ToWStr(temp_buf, temp_len, wtemp_buf, wtemp_len);
    CharToOemBuffW(&wtemp_buf[0], &temp_buf[0], uint32(wtemp_len + 1));
    fprintf(out, "%s", temp_buf);
#else
    vfprintf(out, str, *ap);
#endif
}

bool Utf8ToUpperOnlyLatin(std::string& utf8String) {
    std::wstring wstr;
    if(!Utf8ToWStr(utf8String, wstr))
        return false;

    std::transform(wstr.begin(), wstr.end(), wstr.begin(), wcharToUpperOnlyLatin);

    return WStrToUtf8(wstr, utf8String);
}

void wstringToUpper(std::wstring& str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), wcharToUpper);
}
void wstringToLower(std::wstring& str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), wcharToLower);
}

void stringToUpper(std::string& str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), charToUpper);
}
void stringToLower(std::string& str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), charToLower);
}