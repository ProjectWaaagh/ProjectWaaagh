#include "Appender.hpp"
#include "LogMessage.hpp"
#include "StringFormat.hpp"

#include <sstream>

Appender::Appender(uint8 _id, std::string const& _name, LogLevel _level /*LOG_LEVEL_DISABLED*/, AppenderFlags _flags /*APPENDER_FLAGS_NONE*/) : 
			mID(_id), mName(_name), mLevel(_level), mFlags(_flags)
{}

Appender::~Appender() {}

uint8 Appender::GetID() const {
	return mID;
}

std::string const& Appender::GetName() const {
	return mName;
}

LogLevel Appender::GetLogLevel() const {
	return mLevel;
}

AppenderFlags Appender::GetFlags() const {
	return mFlags;
}

void Appender::SetLogLevel(LogLevel level) {
	level = mLevel;
}

void Appender::Write(LogMessage* message) {
	if (!mLevel || mLevel > message->level)
		return;

	std::ostringstream ss;

	if (mFlags & APPENDER_FLAGS_PREFIX_TIMESTAMP)
		ss << message->GetTimeStr() << ' ';

	if (mFlags & APPENDER_FLAGS_PREFIX_LOGLEVEL)
		ss << ProjectWaaagh::StringFormat("%-5s ", Appender::GetLogLevelString(message->level));

	if (mFlags & APPENDER_FLAGS_PREFIX_LOGFILTERTYPE)
		ss << '[' << message->type << "] ";

	message->prefix = ss.str();
	_Write(message);
}

char const* Appender::GetLogLevelString(LogLevel level) {
	switch (level) {
	case LOG_LEVEL_FATAL:
		return "FATAL";
	case LOG_LEVEL_ERROR:
		return "ERROR";
	case LOG_LEVEL_WARN:
		return "WARN";
	case LOG_LEVEL_INFO:
		return "INFO";
	case LOG_LEVEL_DEBUG:
		return "DEBUG";
	case LOG_LEVEL_TRACE:
		return "TRACE";
	default:
		return "DISABLED";
	}
}
