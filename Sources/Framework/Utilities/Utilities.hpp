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

PRW_FRAMEWORK_API struct tm* localtime_r(time_t const* time, struct tm *result);

PRW_FRAMEWORK_API bool StringEqualI(std::string_view str1, std::string_view str2);


PRW_FRAMEWORK_API size_t Utf8Length(std::string& Utf8Str);
PRW_FRAMEWORK_API void Utf8Truncate(std::string& Utf8Str, size_t length);

PRW_FRAMEWORK_API bool Utf8ToWStr(std::string_view utf8Str, std::wstring& WStr);
PRW_FRAMEWORK_API bool Utf8ToWStr(char const* utf8Str, size_t cSize, wchar_t* WStr, size_t& WSize);
inline bool Utf8toWStr(std::string_view utf8Str, wchar_t* WStr, size_t& WSize) {
    return Utf8ToWStr(utf8Str.data(), utf8Str.size(), WStr, WSize);
}

PRW_FRAMEWORK_API bool WStrToUtf8(std::wstring_view WStr, std::string& Utf8Str);
PRW_FRAMEWORK_API bool WStrToUtf8(wchar_t const* WStr, size_t Size, std::string& Utf8Str);

PRW_FRAMEWORK_API bool utf8ToConsole(std::string_view utf8str, std::string& conStr);
PRW_FRAMEWORK_API bool consoleToUtf8(std::string_view conStr, std::string& utf8Str);
PRW_FRAMEWORK_API bool Utf8FitTo(std::string_view str, std::wstring_view search);
PRW_FRAMEWORK_API void utf8printf(FILE* out, const char* str, ...);
PRW_FRAMEWORK_API void vutf8printf(FILE* out, const char* str, va_list* ap);
PRW_FRAMEWORK_API bool Utf8ToUpperOnlyLatin(std::string& utf8String);

inline bool isBasicLatinCharacter(wchar_t wchar) {
    if(wchar >= L'a' && wchar <= L'z')
        return true;
    if(wchar >= L'A' && wchar <= L'Z')
        return true;
    return false;
}
inline bool isExtendedLatinCharacter(wchar_t wchar) {
    if (isBasicLatinCharacter(wchar))
        return true;
    if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
        return true;
    if (wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
        return true;
    if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
        return true;
    if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
        return true;
    if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
        return true;
    if (wchar >= 0x0100 && wchar <= 0x012F)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK
        return true;
    if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
        return true;
    return false;
}

inline bool isCyrillicCharacter(wchar_t wchar) {
    if (wchar >= 0x0410 && wchar <= 0x044F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC SMALL LETTER YA
        return true;
    if (wchar == 0x0401 || wchar == 0x0451)                  // CYRILLIC CAPITAL LETTER IO, CYRILLIC SMALL LETTER IO
        return true;
    return false;
}

inline bool isEastAsianCharacter(wchar_t wchar) {
    if (wchar >= 0x1100 && wchar <= 0x11F9)                  // Hangul Jamo
        return true;
    if (wchar >= 0x3041 && wchar <= 0x30FF)                  // Hiragana + Katakana
        return true;
    if (wchar >= 0x3131 && wchar <= 0x318E)                  // Hangul Compatibility Jamo
        return true;
    if (wchar >= 0x31F0 && wchar <= 0x31FF)                  // Katakana Phonetic Ext.
        return true;
    if (wchar >= 0x3400 && wchar <= 0x4DB5)                  // CJK Ideographs Ext. A
        return true;
    if (wchar >= 0x4E00 && wchar <= 0x9FC3)                  // Unified CJK Ideographs
        return true;
    if (wchar >= 0xAC00 && wchar <= 0xD7A3)                  // Hangul Syllables
        return true;
    if (wchar >= 0xFF01 && wchar <= 0xFFEE)                  // Halfwidth forms
        return true;
    return false;
}

inline bool isNumeric(wchar_t wchar) {
    return (wchar >= L'0' && wchar <=L'9');
}

inline bool isNumeric(char c) {
    return (c >= '0' && c <='9');
}

inline bool isNumeric(char const* str) {
    for (char const* c = str; *c; ++c)
        if (!isNumeric(*c))
            return false;

    return true;
}

inline bool isNumericOrSpace(wchar_t wchar) {
    return isNumeric(wchar) || wchar == L' ';
}

inline bool isBasicLatinString(std::wstring_view wstr, bool numericOrSpace) {
    for (wchar_t c : wstr)
        if (!isBasicLatinCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
            return false;
    return true;
}

inline bool isExtendedLatinString(std::wstring_view wstr, bool numericOrSpace) {
    for (wchar_t c : wstr)
        if (!isExtendedLatinCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
            return false;
    return true;
}

inline bool isCyrillicString(std::wstring_view wstr, bool numericOrSpace) {
    for (wchar_t c : wstr)
        if (!isCyrillicCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
            return false;
    return true;
}

inline bool isEastAsianString(std::wstring_view wstr, bool numericOrSpace) {
    for (wchar_t c : wstr)
        if (!isEastAsianCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
            return false;
    return true;
}

inline wchar_t wcharToUpper(wchar_t wchar) {
    if (wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
        return wchar_t(0x1E9E);
    if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar >= 0x0101 && wchar <= 0x012F) {
        if (wchar % 2 == 1)
            return wchar_t(uint16(wchar)-0x0001);
    }
    if (wchar >= 0x0430 && wchar <= 0x044F)                  // CYRILLIC SMALL LETTER A - CYRILLIC SMALL LETTER YA
        return wchar_t(uint16(wchar)-0x0020);
    if (wchar == 0x0451)                                     // CYRILLIC SMALL LETTER IO
        return wchar_t(0x0401);
    if (wchar == 0x0153)                                     // LATIN SMALL LIGATURE OE
        return wchar_t(0x0152);
    if (wchar == 0x00FF)                                     // LATIN SMALL LETTER Y WITH DIAERESIS
        return wchar_t(0x0178);

    return wchar;
}

inline wchar_t wcharToUpperOnlyLatin(wchar_t wchar) {
    return isBasicLatinCharacter(wchar) ? wcharToUpper(wchar) : wchar;
}

inline wchar_t wcharToLower(wchar_t wchar) {
    if (wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
        return wchar_t(uint16(wchar)+0x0020);
    if (wchar >= 0x0100 && wchar <= 0x012E) {
        if (wchar % 2 == 0)
            return wchar_t(uint16(wchar)+0x0001);
    }
    if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
        return wchar_t(0x00DF);
    if (wchar == 0x0401)                                     // CYRILLIC CAPITAL LETTER IO
        return wchar_t(0x0451);
    if (wchar == 0x0152)                                     // LATIN CAPITAL LIGATURE OE
        return wchar_t(0x0153);
    if (wchar == 0x0178)                                     // LATIN CAPITAL LETTER Y WITH DIAERESIS
        return wchar_t(0x00FF);
    if (wchar >= 0x0410 && wchar <= 0x042F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC CAPITAL LETTER YA
        return wchar_t(uint16(wchar)+0x0020);

    return wchar;
}

inline bool isLower(wchar_t wchar) {
    if (wchar >= L'a' && wchar <= L'z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
        return true;
    if (wchar >= 0x00E0 && wchar <= 0x00FF)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER Y WITH DIAERESIS
        return true;
    if (wchar >= 0x0430 && wchar <= 0x044F)                  // CYRILLIC SMALL LETTER A - CYRILLIC SMALL LETTER YA
        return true;
    if (wchar == 0x0451)                                     // CYRILLIC SMALL LETTER IO
        return true;
    return false;
}

inline bool isUpper(wchar_t wchar) {
    return !isLower(wchar);
}

inline char charToUpper(char c) { return std::toupper(c); }
inline char charToLower(char c) { return std::tolower(c); }

PRW_FRAMEWORK_API void wstringToUpper(std::wstring& str);
PRW_FRAMEWORK_API void wstringToLower(std::wstring& str);
PRW_FRAMEWORK_API void strToUpper(std::string& str);
PRW_FRAMEWORK_API void strToLower(std::string& str);


#endif //PROJECTWAAAGH_UTILITIES_HPP
