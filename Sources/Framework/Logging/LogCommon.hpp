#ifndef PROJECT_WAAAGH_FRAMEWORK_LOGCOMMON_HPP
#define PROJECT_WAAAGH_FRAMEWORK_LOGCOMMON_HPP

/**
 * @brief Common Log Enumerations
 * @author Atlata
 * @copyright (c) 2020 - 2021 by ProjectWaaagh
 * 
 * @details description
 */

enum LogLevel {
    LOG_LEVEL_DISABLED  = 0,
    LOG_LEVEL_TRACE     = 1,
    LOG_LEVEL_DEBUG     = 2,
    LOG_LEVEL_INFO      = 3,
    LOG_LEVEL_WARN      = 4,
    LOG_LEVEL_ERROR     = 5,
    LOG_LEVEL_FATAL     = 6,

    NUM_ENABLED_LOG_LEVELS = 6
};

enum LogType {
    LOG_TYPE_NONE,
    LOG_TYPE_CONSOLE,
    LOG_TYPE_FILE,
    LOG_TYPE_DB,
    LOG_TYPE_TCP
};

enum LogFlags {
    LOG_FLAGS_NONE                  = 0x00,
    LOG_FLAGS_PREFIX_TIMESTAMP      = 0x01,
    LOG_FLAGS_PREFIX_LOGLEVEL       = 0x02,
    LOG_FLAGS_PREFIX_LOGFILTERTYPE  = 0x04,
    LOG_FLAGS_USE_TIMESTAMP         = 0x08,
    LOG_FLAGS_MAKE_FILE_BACKUP      = 0x10,

};

#endif //PROJECT_WAAAGH_FRAMEWORK_LOGCOMMON_HPP