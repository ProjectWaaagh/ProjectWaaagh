#ifndef PROJECT_WAAAGH_FRAMEWORK_LOGOPERATION_HPP
#define PROJECT_WAAAGH_FRAMEWORK_LOGOPERATION_HPP

#include "Defines.hpp"
#include <memory>

class Logger;
class LogMessage;

class PW_FRAMEWORK_API LogOperation {
public:
    LogOperation();
    ~LogOperation();

    int Call();
    
protected:
    Logger const* logger;
    std::unique_ptr<LogMessage> message;

};

#endif //PROJECT_WAAAGH_FRAMEWORK_LOGOPERATION_HPP