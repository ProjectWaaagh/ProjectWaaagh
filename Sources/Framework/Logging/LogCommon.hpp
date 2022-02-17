#ifndef PROJECTWAAAGH_LOGCOMMON_HPP
#define PROJECTWAAAGH_LOGCOMMON_HPP

#include "Defines.hpp"

enum LogLevel : uint8 {

	LOG_LEVEL_DISABLED	= 0,
	LOG_LEVEL_TRACE		= 1,
	LOG_LEVEL_DEBUG		= 2,
	LOG_LEVEL_INFO		= 3,
	LOG_LEVEL_WARN		= 4,
	LOG_LEVEL_ERROR		= 5,
	LOG_LEVEL_FATAL		= 6,

	NUM_ENABLED_LOG_LEVELS = LOG_LEVEL_FATAL,
	LOG_LEVEL_INVALID = 0xFF
};

enum AppenderType : uint8 {

	APPENDER_NONE,
	APPENDER_CONSOLE,
	APPENDER_FILE,
	APPENDER_DATABASE,

	APPENDER_INVALID = 0xFF
};

enum AppenderFlags : uint8 {

	APPENDER_FLAGS_NONE						= 0x00,
	APPENDER_FLAGS_PREFIX_TIMESTAMP			= 0x01,
	APPENDER_FLAGS_PREFIX_LOGLEVEL			= 0x02,
	APPENDER_FLAGS_PREFIX_LOGFILTERTYPE		= 0x04,
	APPENDER_FLAGS_USE_TIMESTAMP			= 0x08,
	APPENDER_FLAGS_MAKE_FILE_BACKUP			= 0x10,
};

#endif // !PROJECTWAAAGH_LOGCOMMON_HPP
