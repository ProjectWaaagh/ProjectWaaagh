#include "LogMessage.hpp"
#include "StringFormat.hpp"
#include "Utilities.hpp"

LogMessage::LogMessage(LogLevel mlogLevel, std::string const& mtype, std::string&& mlogText) :
    logLevel(mlogLevel), type(mtype), logText(std::forward<std::string>(mlogText)), mtime(time(nullptr)) {}
    
LogMessage::LogMessage(LogLevel mlogLevel, std::string const& mtype, std::string&& mlogText, std::string&& mlogParam1) :
    logLevel(mlogLevel), type(mtype), logText(std::forward<std::string>(mlogText)), param1(std::forward<std::string>(mlogParam1)), mtime(time(nullptr)) {}

std::string LogMessage::GetTimeString(time_t time) {
    tm aTm;
    localtime_r(&time, &aTm);

    return ProjectWaaagh::StringFormat("%04d-%02d-%02d_%02d:%02d:%02d",
                        aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_mday, aTm.tm_sec);
}

std::string LogMessage::GetTimeString() const {
    return GetTimeString(mtime);
}