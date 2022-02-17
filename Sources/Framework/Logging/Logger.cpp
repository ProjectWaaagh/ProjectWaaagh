#include "Logger.hpp"
#include "Appender.hpp"
#include "LogMessage.hpp"


Logger::Logger(std::string const& name, LogLevel level) : mName(name), mLevel(level) {}

std::string const& Logger::GetName() const {
	return mName;
}

LogLevel Logger::GetLogLevel() const {
	return mLevel;
}

void Logger::AddAppender(uint8 type, Appender* appender) {
	mAppenders[type] = appender;
}

void Logger::DelAppender(uint8 type) {
	mAppenders.erase(type);
}

void Logger::SetLogLevel(LogLevel level) {
	mLevel = level;
}

void Logger::Write(LogMessage* message) const {
	if (!mLevel || mLevel > message->level || message->text.empty()) {
		fprintf(stderr, "Logger::Write: Logger %s, level %u, MSG %s Level %u WRONG LEVEL MASK OR EMPTY MSG\n",
			GetName().c_str(), GetLogLevel(), message->text.c_str(), message->level);
	}

	for (std::pair<uint8 const, Appender*> const& appender : mAppenders)
		if (appender.second)
			appender.second->Write(message);
}
