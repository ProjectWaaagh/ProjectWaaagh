#ifndef PROJECTWAAAGH_LOGOPERATION_HPP
#define PROJECTWAAAGH_LOGOPERATION_HPP

#include "Defines.hpp"
#include "Logger.hpp"
#include "LogMessage.hpp"

#include <memory>

class PRW_FRAMEWORK_API LogOperation {
public:
    LogOperation(Logger const* _logger, std::unique_ptr<LogMessage>&& _msg);

    ~LogOperation();

    int call();

protected:
    Logger const* logger;
    std::unique_ptr<LogMessage> msg;
};

#endif //PROJECTWAAAGH_LOGOPERATION_HPP