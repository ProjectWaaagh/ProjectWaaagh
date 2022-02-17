#ifndef PROJECTWAAAGH_LOGMESSAGE_HPP
#define PROJECTWAAAGH_LOGMESSAGE_HPP

#include "Defines.hpp"
#include "LogCommon.hpp"

#include <string>
#include <ctime>

struct PRW_FRAMEWORK_API LogMessage {
	LogMessage(LogLevel _level, std::string const& _type, std::string&& _text);
	LogMessage(LogLevel _level, std::string const& _type, std::string&& _text, std::string&& _param1);

	LogMessage(LogMessage const& /*other*/) = delete;
	LogMessage& operator=(LogMessage const& /*other*/) = delete;

	static std::string GetTimeStr(time_t time);
	std::string GetTimeStr() const;

	LogLevel const level;
	std::string const type;
	std::string const text;
	std::string prefix;
	std::string param1;
	time_t mTime;

	uint32 Size() const {
		return static_cast<uint32>(prefix.size() + text.size());
	}
};

#endif // !PROJECTWAAAGH_LOGMESSAGE_HPP
