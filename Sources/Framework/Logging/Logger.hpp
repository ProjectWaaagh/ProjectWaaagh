#ifndef PROJECTWAAAGH_LOGGER_HPP
#define PROJECTWAAAGH_LOGGER_HPP

#include "Defines.hpp"
#include "LogCommon.hpp"

#include <unordered_map>
#include <string>

class Appender;
struct LogMessage;

class PRW_FRAMEWORK_API Logger {
public:
	Logger(std::string const& name, LogLevel level);

	void AddAppender(uint8 type, Appender* appender);
	void DelAppender(uint8 type);

	std::string const& GetName() const;
	LogLevel GetLogLevel() const;
	void SetLogLevel(LogLevel level);
	void Write(LogMessage* message) const;

private:
	std::string mName;
	LogLevel mLevel;
	std::unordered_map<uint8, Appender*> mAppenders;
};

#endif // !PROJECTWAAAGH_LOGGER_HPP
