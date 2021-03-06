#include "LogMessage.hpp"
#include "StringFormat.hpp"
#include "Utilities.hpp"

#include <time.h>

LogMessage::LogMessage(LogLevel _level, std::string const& _type, std::string&& _text) :
		level(_level), type(_type), text(std::forward<std::string>(_text)), mTime(time(nullptr)) 
{}

LogMessage::LogMessage(LogLevel _level, std::string const& _type, std::string&& _text, std::string&& _param1) :
	level(_level), type(_type), text(std::forward<std::string>(_text)), param1(std::forward<std::string>(_param1)), mTime(time(nullptr))
{}

std::string LogMessage::GetTimeStr(time_t time) {

	tm aTm;
	localtime_s(&aTm, &time);
	return ProjectWaaagh::StringFormat("%04d-%02d-%02d_%02d:%02d:02d",
		aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_min, aTm.tm_sec);
}

std::string LogMessage::GetTimeStr() const {
	return GetTimeStr(mTime);
}
