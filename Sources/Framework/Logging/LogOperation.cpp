#include "LogOperation.hpp"
#include "Logger.hpp"
#include "LogMessage.hpp"

LogOperation::LogOperation(Logger const* _logger, std::unique_ptr<LogMessage>&& _msg) : logger(_logger), msg(std::forward<std::unique_ptr<LogMessage>>(_msg)) {

}

LogOperation::~LogOperation() {}

int LogOperation::call() {
    logger->Write(msg.get());
    return 0;
}