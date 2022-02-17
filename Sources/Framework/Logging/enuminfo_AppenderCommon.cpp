#include "LogCommon.hpp"
#include "Defines.hpp"
#include "SmartEnum.hpp"
#include <stdexcept>

namespace ProjectWaaagh::Impl::EnumUtilsImpl {

    template<>
    PRW_API_EXPORT EnumText EnumUtils<LogLevel>::ToString(LogLevel value) {
        switch (value) {
            case LOG_LEVEL_DISABLED: return { "LOG_LEVEL_DISABLED", "LOG_LEVEL_DISABLED", "" };
            case LOG_LEVEL_TRACE: return { "LOG_LEVEL_TRACE", "LOG_LEVEL_TRACE", "" };
            case LOG_LEVEL_DEBUG: return { "LOG_LEVEL_DEBUG", "LOG_LEVEL_DEBUG", "" };
            case LOG_LEVEL_INFO: return { "LOG_LEVEL_INFO", "LOG_LEVEL_INFO", "" };
            case LOG_LEVEL_WARN: return { "LOG_LEVEL_WARN", "LOG_LEVEL_WARN", "" };
            case LOG_LEVEL_ERROR: return { "LOG_LEVEL_ERROR", "LOG_LEVEL_ERROR", "" };
            case LOG_LEVEL_FATAL: return { "LOG_LEVEL_FATAL", "LOG_LEVEL_FATAL", "" };
            default: throw std::out_of_range("value");
        }
    }

    template<>
    PRW_API_EXPORT size_t EnumUtils<LogLevel>::Count() { return 7; }

    template<>
    PRW_API_EXPORT LogLevel EnumUtils<LogLevel>::FromIndex(size_t index) {
        switch (index) {
            case 0: return LOG_LEVEL_DISABLED;
            case 1: return LOG_LEVEL_TRACE;
            case 2: return LOG_LEVEL_DEBUG;
            case 3: return LOG_LEVEL_INFO;
            case 4: return LOG_LEVEL_WARN;
            case 5: return LOG_LEVEL_ERROR;
            case 6: return LOG_LEVEL_FATAL;

            default: throw std::out_of_range("index");
        }
    }

    template<>
    PRW_API_EXPORT size_t EnumUtils<LogLevel>::ToIndex(LogLevel index) {
        switch(index) {
            case LOG_LEVEL_DISABLED: return 0;
            case LOG_LEVEL_TRACE: return 1;
            case LOG_LEVEL_DEBUG: return 2;
            case LOG_LEVEL_INFO: return 3;
            case LOG_LEVEL_WARN: return 4;
            case LOG_LEVEL_ERROR: return 5;
            case LOG_LEVEL_FATAL: return 6;
            default: throw std::out_of_range("index");
        }
    }



    template<>
    PRW_API_EXPORT EnumText EnumUtils<AppenderType>::ToString(AppenderType value) {
        switch (value) {
            case APPENDER_NONE: return { "APPENDER_NONE", "APPENDER_NONE", "" };
            case APPENDER_CONSOLE: return { "APPENDER_CONSOLE", "APPENDER_CONSOLE", "" };
            case APPENDER_FILE: return { "APPENDER_FILE", "APPENDER_FILE", "" };
            case APPENDER_DATABASE: return { "APPENDER_DATABASE", "APPENDER_DATABASE", "" };
            default: throw std::out_of_range("value");

        }
    }

    template<>
    PRW_API_EXPORT size_t EnumUtils<AppenderType>::Count() { return 4; }

    template<>
    PRW_API_EXPORT AppenderType EnumUtils<AppenderType>::FromIndex(size_t index) {
        switch(index) {
            case 0: return APPENDER_NONE;
            case 1: return APPENDER_CONSOLE;
            case 2: return APPENDER_FILE;
            case 3: return APPENDER_DATABASE;
            default: throw std::out_of_range("index");
        }
    }

    template<>
    PRW_API_EXPORT size_t EnumUtils<AppenderType>::ToIndex(AppenderType index) {
        switch(index) {
            case APPENDER_NONE: return 0;
            case APPENDER_CONSOLE: return 1;
            case APPENDER_FILE: return 2;
            case APPENDER_DATABASE: return 3;
            default: throw std::out_of_range("index");
        }
    }
} //!ProjectWaaagh::Impl::EnumUtilsImpl