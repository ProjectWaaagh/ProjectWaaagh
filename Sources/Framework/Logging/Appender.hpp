#ifndef PROJECTWAAAGH_APPENDER_HPP
#define PROJECTWAAAGH_APPENDER_HPP

#include "Defines.hpp"
#include "LogCommon.hpp"

#include <stdexcept>
#include <string>
#include <vector>

struct LogMessage;

class PRW_FRAMEWORK_API Appender {
public:
	Appender(uint8 _id, std::string const& name, LogLevel level = LOG_LEVEL_DISABLED, AppenderFlags flags = APPENDER_FLAGS_NONE);
	virtual ~Appender();

	uint8 GetID() const;
	std::string const& GetName() const;
	virtual AppenderType GetType() const = 0;
	LogLevel GetLogLevel() const;
	AppenderFlags GetFlags() const;

	void SetLogLevel(LogLevel);
	void Write(LogMessage* message);
	static char const* GetLogLevelString(LogLevel level);
	
private:
	virtual void _Write(LogMessage const* /*message*/) = 0;

	uint8 mID;
	std::string mName;
	LogLevel mLevel;
	AppenderFlags mFlags;
};

class PRW_FRAMEWORK_API InvalidAppenderArgsException : public std::length_error {
public:
	explicit InvalidAppenderArgsException(std::string const& message) : std::length_error(message) {}
};

#endif // !PROJECTWAAAGH_APPENDER_HPP
