#ifndef PROJECT_WAAAGH_FRAMEWORK_LOGMESSAGE_HPP
#define PROJECT_WAAAGH_FRAMEWORK_LOGMESSAGE_HPP

/**
 * @brief LogMessage Struct
 * @author Atlata
 * @copyright (c) 2020 - 2021 by ProjectWaaagh
 * 
 * @details description
 */

#include "LogCommon.hpp"
#include <string>
#include <ctime>

struct LogMessage {

    /**
     * @brief Construct a new Log Message object
     * 
     * @param mlogLevel The logging level enumeration
     * @param mtype     The Logging type enumartion
     * @param mlogText  The Loggin Text
     */
    LogMessage(LogLevel mlogLevel, std::string const& mtype, std::string&& mlogText);

    /**
     * @brief Construct a new Log Message object
     * 
     * @param mlogLevel The logging level enumeration
     * @param mtype     The Logging type enumartion
     * @param mlogText  The Loggin Text
     * @param mlogParam The Logggin Parameters
     */
    LogMessage(LogLevel mlogLevel, std::string const& mtype, std::string&& mlogText, std::string&& mlogParam1);

    /**
     * @brief Default deconstructor
     * 
     */
    LogMessage(LogMessage const& /*OTHER*/) = delete;
    LogMessage& operator = (LogMessage const& /*OTHER*/) = delete;

    /**
     * @brief Get the Time String object
     * 
     * @param time 
     * @return time std::string 
     */
    static std::string GetTimeString(time_t time);
    std::string GetTimeString() const;

    LogLevel const logLevel;
    std::string const type;
    std::string const logText;
    std::string prefix;
    std::string param1;
    time_t mtime;

    uint32_t Size() const {
        return static_cast<uint32_t>(prefix.size() + logText.size());
    }
};


#endif