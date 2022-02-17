#include "Log.hpp"
#include "AppenderConsole.hpp"
#include "AppenderFile.hpp"
#include "Framework.hpp"
#include "ConfigManager.hpp"
#include "Errors.hpp"
#include "Logger.hpp"
#include "LogMessage.hpp"
#include "LogOperation.hpp"
#include "Strand.hpp"
#include "StringConverter.hpp"
#include "Utilities.hpp"

#include <chrono>
#include <sstream>

Log::Log() : AppenderID(0), lowestLogLevel(LOG_LEVEL_FATAL), mIoContext(nullptr), mStrand(nullptr) {
    mLogsTimestamp = "_" + GetTimestampStr();
    RegisterAppender<AppenderConsole>();
    RegisterAppender<AppenderFile>();
}

Log::~Log() {
    delete mStrand;
    Close();
}

uint8 Log::NextAppenderID() {
    return AppenderID++;
}

Appender *Log::GetAppenderByName(std::string_view name) {
    auto it = appenders.begin();
    while(it != appenders.end() && it->second && it->second->GetName() != name)
        ++it;

    return it == appenders.end() ? nullptr : it->second.get();
}

void Log::CreateAppenderFromConfigLine(const std::string &name, const std::string &options) {
    if(name.empty())
        return;

    std::vector<std::string_view> tokens = ProjectWaaagh::Tokenize(options, ',', true);
    size_t const size = tokens.size();
    std::string appenderName = name.substr(9);

    if(size < 2) {
        fprintf(stderr, "Log::CreteAppenderFromConfig: Wrong configuration to appender %s. Config line: %s\n", appenderName.c_str(), options.c_str());
        return;
    }

    AppenderFlags flags = APPENDER_FLAGS_NONE;
    AppenderType type = AppenderType(ProjectWaaagh::StringTo<uint8>(tokens[0]).value_or(APPENDER_INVALID));
    LogLevel level = LogLevel(ProjectWaaagh::StringTo<uint8>(tokens[1]).value_or(LOG_LEVEL_INVALID));

    auto factoryFunction = appenderFactory.find(type);
    if(factoryFunction == appenderFactory.end()) {
        fprintf(stderr, "Log::CreateAppenderFromConfig: Unknown type '%s' for appender %s\n", std::string(tokens[0]).c_str(), appenderName.c_str());
        return;
    }

    if(level > NUM_ENABLED_LOG_LEVELS) {
        fprintf(stderr, "Log::CreateAppenderFromConfig: Wrong Log level '%s' for appender %s\n", std::string(tokens[1]).c_str(), appenderName.c_str());
        return;
    }

    if(size > 2) {
        if(Optional<uint8> flagsVal = ProjectWaaagh::StringTo<uint8>(tokens[2]))
            flags = AppenderFlags(*flagsVal);
        else {
            fprintf(stderr, "Log::CreateAppenderFromConfig: Unknown flags '%s' for appender %s\n", std::string(tokens[2]).c_str(), appenderName.c_str());
            return;
        }
    }

    try {
        Appender* appender = factoryFunction->second(NextAppenderID(), appenderName, level, flags, tokens);
        appenders[appender->GetID()].reset(appender);
    } catch (InvalidAppenderArgsException const& iaae) {
        fprintf(stderr, "%s\n", iaae.what());
    }
}

void Log::CreateAppenderFromConfig(const std::string &name) {
    CreateAppenderFromConfigLine(name, sConfigManager->GetStringDefault(name, ""));
}

void Log::CreateLoggerFromConfigLine(const std::string &name, const std::string &options) {
    if(name.empty())
        return;

    LogLevel level = LOG_LEVEL_DISABLED;

    std::string loggerName = name.substr(7);

    if(options.empty()) {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Missing config option Logger.%s\n", loggerName.c_str());
        return;
    }

    std::vector<std::string_view> tokens = ProjectWaaagh::Tokenize(options, ',', true);
    if(tokens.size() != 5) {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong config option Logger.%s=%s\n", loggerName.c_str(), options.c_str());
        return;
    }

    std::unique_ptr<Logger>& logger = loggers[loggerName];
    if(logger) {
        fprintf(stderr, "Error while configuring Logger %s. Already defined\n", loggerName.c_str());
    }

    level = LogLevel(ProjectWaaagh::StringTo<uint8>(tokens[0]).value_or(LOG_LEVEL_INVALID));
    if(level > NUM_ENABLED_LOG_LEVELS) {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong Log Level '%s' for logger %s\n", std::string(tokens[0]).c_str(), loggerName.c_str());
        return;
    }

    if(level < lowestLogLevel)
        lowestLogLevel = level;

    logger = std::make_unique<Logger>(loggerName, level);

    for(std::string_view appenderName : ProjectWaaagh::Tokenize(tokens[1], ' ', false)) {
        if(Appender* appender = GetAppenderByName(appenderName)) {
            logger->AddAppender(appender->GetID(), appender);
        } else
            fprintf(stderr, "Error while configuring Appender %s in Logger %s. Appender does not exist\n", std::string(appenderName).c_str(), loggerName.c_str());
    }
}

void Log::CreateLoggerFromConfig(const std::string &name) {
    CreateLoggerFromConfigLine(name, sConfigManager->GetStringDefault(name, ""));
}

void Log::ReadAppendersFromConfig() {
    std::vector<std::string> keys = sConfigManager->GetKeysByString("Appender.");
    for(std::string const& appenderName : keys)
        CreateAppenderFromConfig(appenderName);
}

void Log::ReadLoggersFromConfig() {
    std::vector<std::string> keys = sConfigManager->GetKeysByString("Logger.");
    for(std::string const& loggerName : keys)
        CreateLoggerFromConfig(loggerName);

    if(loggers.find(LOGGER_ROOT) == loggers.end()) {
        fprintf(stderr, "Wrong Loggers configuration. Review your Logger config Section.\nCreating default loggers [root (Error), server (Info)] to console.\n");
        Close();

        AppenderConsole* appender = new AppenderConsole(NextAppenderID(), "Console", LOG_LEVEL_DEBUG, APPENDER_FLAGS_NONE, {});
        appenders[appender->GetID()].reset(appender);

        Logger* rootLogger = new Logger(LOGGER_ROOT, LOG_LEVEL_ERROR);
        rootLogger->AddAppender(appender->GetID(), appender);
        loggers[LOGGER_ROOT].reset(rootLogger);

        Logger* serverLogger = new Logger("server", LOG_LEVEL_INFO);
        serverLogger->AddAppender(appender->GetID(), appender);
        loggers["server"].reset(serverLogger);
    }
}

void Log::RegisterAppender(uint8 index, AppenderCreatorFn appenderCreatorFn) {
    auto itr = appenderFactory.find(index);
    ASSERT(itr == appenderFactory.end());
    appenderFactory[index] = appenderCreatorFn;
}

void Log::outMessage(const std::string &filter, LogLevel level, std::string &&message) {
    write(std::make_unique<LogMessage>(level, filter, std::move(message)));
}

void Log::outCommand(std::string &&message, std::string &&param1) {
    write(std::make_unique<LogMessage>(LOG_LEVEL_INFO, "Commands.GM", std::move(message), std::move(param1)));
}

void Log::write(std::unique_ptr<LogMessage> &&msg) const {
    Logger const* logger = GetLoggerByType(msg->type);

    if(mIoContext) {
        std::shared_ptr<LogOperation> logOperation = std::make_shared<LogOperation>(logger, std::move(msg));
        ProjectWaaagh::Asio::post(*mIoContext, ProjectWaaagh::Asio::bind_executor(*mStrand, [logOperation]() { logOperation->call(); }));
    } else
        logger->Write(msg.get());
}

const Logger *Log::GetLoggerByType(const std::string &type) const {
    auto it = loggers.find(type);
    if(it != loggers.end())
        return it->second.get();

    if(type == LOGGER_ROOT)
        return nullptr;

    std::string parentLogger = LOGGER_ROOT;
    size_t found = type.find_last_of('.');
    if(found != std::string::npos)
        parentLogger = type.substr(0, found);

    return GetLoggerByType(parentLogger);
}

std::string Log::GetTimestampStr() {
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::tm aTm;
    localtime_r(&tt, &aTm);

    try {
        return ProjectWaaagh::StringFormat("%04d-%02d-%02d_%02d-%02d-%02d",
            aTm.tm_year + 1900, aTm.tm_mon + 1, aTm.tm_mday, aTm.tm_hour, aTm.tm_min, aTm.tm_sec);
    } catch (std::exception const& ex) {
        fprintf(stderr, "Failed to initialize timestamp part of log filename! %s", ex.what());
        fflush(stderr);
        ABORT();
    }
}

bool Log::SetLogLevel(const std::string &name, int32 level, bool isLogger) {
    if(level < 0)
        return false;

    LogLevel newLevel = LogLevel(level);

    if(isLogger) {
        auto it = loggers.begin();
        while(it != loggers.end() && it->second->GetName()!= name)
            ++it;

        if(it == loggers.end())
            return false;

        it->second->SetLogLevel(newLevel);

        if(newLevel != LOG_LEVEL_DISABLED && newLevel < lowestLogLevel)
            lowestLogLevel = newLevel;
    } else {
        Appender* appender = GetAppenderByName(name);
        if(!appender)
            return false;

        appender->SetLogLevel(newLevel);
    }
    return true;
}

void Log::Close() {
    loggers.clear();
    appenders.clear();
}

bool Log::ShouldLog(const std::string &type, LogLevel level) const {
    if(level < lowestLogLevel)
        return false;

    Logger const* logger = GetLoggerByType(type);
    if(!logger)
        return false;

    LogLevel  logLevel = logger->GetLogLevel();

    return logLevel != LOG_LEVEL_DISABLED && logLevel <= level;
}

Log* Log::Instance() {
    static Log Instance;
    return &Instance;
}

void Log::Initialize(ProjectWaaagh::Asio::IoContext *ioContext) {
    if(ioContext) {
        mIoContext = ioContext;
        mStrand = new ProjectWaaagh::Asio::Strand(*ioContext);
    }

    LoadFromConfig();
}

void Log::SetSynchronous() {
    delete mStrand;
    mStrand = nullptr;
    mIoContext = nullptr;
}

void Log::LoadFromConfig() {
    Close();

    lowestLogLevel = LOG_LEVEL_FATAL;
    AppenderID = 0;
    mLogsDir = sConfigManager->GetStringDefault("LogsDir", "");
    if(!mLogsDir.empty())
        if((mLogsDir.at(mLogsDir.length() - 1) != '/') && (mLogsDir.at(mLogsDir.length() -1) != '\\'))
            mLogsDir.push_back('/');

    ReadAppendersFromConfig();
    ReadLoggersFromConfig();
}