#ifndef PROJECTWAAAGH_LOG_HPP
#define PROJECTWAAAGH_LOG_HPP

#include "Defines.hpp"
#include "AsioHacksForwarder.hpp"
#include "LogCommon.hpp"
#include "StringFormat.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

class Appender;
class Logger;
struct LogMessage;

namespace ProjectWaaagh::Asio {
    class IoContext;
}

#define LOGGER_ROOT "root"

typedef Appender*(*AppenderCreatorFn)(uint8 id, std::string const& name, LogLevel level, AppenderFlags flags, std::vector<std::string_view> const& extraArgs);

template<class AppenderImpl>
Appender* CreateAppender(uint8 id, std::string const& name, LogLevel level, AppenderFlags flags, std::vector<std::string_view> const& extraArgs){
    return new AppenderImpl(id, name, level, flags, extraArgs);
}

class PRW_FRAMEWORK_API Log {
private:
    Log();
    ~Log();
    Log(Log const&) = delete;
    Log(Log&&) = delete;
    Log& operator=(Log const&) = delete;
    Log& operator=(Log&&) = delete;

public:
    static Log* Instance();

    void Initialize(ProjectWaaagh::Asio::IoContext* ioContext);
    void SetSynchronous();
    void LoadFromConfig();
    void Close();

    bool ShouldLog(std::string const& type, LogLevel level) const;
    bool SetLogLevel(std::string const& name, int32 level, bool isLogger = true);

    template<typename Format, typename... Args>
    inline void outMessage(std::string const& filter, LogLevel const level, Format&& fmt, Args&&... args) {
        outMessage(filter,level,ProjectWaaagh::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...));
    }

    template<typename Format, typename... Args>
    void outCommand(uint32 account, Format&& fmt, Args&&... args) {
        if(!ShouldLog("Commands.GM", LOG_LEVEL_INFO))
            return;

        outCommand(ProjectWaaagh::StringFormat(std::forward<Format>(fmt), std::forward<Args>(args)...), std::to_string(account));
    }

    template<class AppenderImpl>
    void RegisterAppender() {
        RegisterAppender(AppenderImpl::type, & CreateAppender<AppenderImpl>);
    }

    std::string const& GetLogsDir() const { return mLogsDir; }
    std::string const& GetLogsTimestamp() const { return mLogsTimestamp; }

    void CreateAppenderFromConfigLine(std::string const& name, std::string const& options);
    void CreateLoggerFromConfigLine(std::string const& name, std::string const& options);

private:
    static std::string GetTimestampStr();
    void write(std::unique_ptr<LogMessage>&& msg) const;

    Logger const* GetLoggerByType(std::string const& type) const;
    Appender* GetAppenderByName(std::string_view name);
    uint8 NextAppenderID();
    void CreateAppenderFromConfig(std::string const& name);
    void CreateLoggerFromConfig(std::string const& name);
    void ReadAppendersFromConfig();
    void ReadLoggersFromConfig();
    void RegisterAppender(uint8 index, AppenderCreatorFn appenderCreatorFn);
    void outMessage(std::string const& filter, LogLevel level, std::string&& message);
    void outCommand(std::string&& message, std::string&& param1);

    std::unordered_map<uint8, AppenderCreatorFn> appenderFactory;
    std::unordered_map<uint8, std::unique_ptr<Appender>> appenders;
    std::unordered_map<std::string, std::unique_ptr<Logger>> loggers;
    uint8 AppenderID;
    LogLevel lowestLogLevel;

    std::string mLogsDir;
    std::string mLogsTimestamp;

    ProjectWaaagh::Asio::IoContext* mIoContext;
    ProjectWaaagh::Asio::Strand* mStrand;
};

#define sLog Log::Instance()

#define LOG_EXCEPTION_FREE(filterType__, level__, ...)                                              \
    {                                                                                               \
        try {                                                                                       \
            sLog->outMessage(filterType__, level__, __VA_ARGS__);                                   \
        } catch (std::exception& e) {                                                               \
            sLog->outMessage("Server", LOG_LEVEL_ERROR, "Wrong format occurred (%s) at %s:%u.",     \
                             e.what(), __FILE__, __LINE__);                                         \
        }                                                                                           \
    }

#ifdef PERFORMANCE_PROFILING
#   define PRW_LOG_MESSAGE_BODY(filterType__, level__, ...) ((void)0)
#elif PROJECTWAAAGH_PLATFORM != PROJECTWAAAGH_PLATFORM_WINDOWS
    void check_args(char const*, ...) ATTR_PRINTF(1, 2);
    void check_args(std::string const&, ...);

#   define PRW_LOG_MESSAGE_BODY(filterType__, level__, ...)             \
        do {                                                            \
            if(sLog->ShouldLog(filterType___, level___)) {              \
                if(false)                                               \
                    check_args(__VA_ARGS__);                            \
                LOG_EXCEPTION_FREE(filterType__, level__, __VA_ARGS__); \
            }                                                           \
        } while(0)
#else
#   define PRW_LOG_MESSAGE_BODY(filterType__, level__, ...)             \
        __pragma(warning(push))                                         \
        __pragma(warning(disable:4127))                                 \
        do {                                                            \
            if(sLog->ShouldLog(filterType__, level__))                  \
                LOG_EXCEPTION_FREE(filterType__, level__, __VA_ARGS__); \
        } while(0)                                                      \
        __pragma(warning(pop))
#endif

#define PRW_LOG_TRACE(filterType__, ...)                                \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_TRACE, __VA_ARGS__)

#define PRW_LOG_DEBUG(filterType__, ...)                                \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_DEBUG, __VA_ARGS__)

#define PRW_LOG_INFO(filterType__, ...)                                 \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_INFO, __VA_ARGS__)

#define PRW_LOG_WARN(filterType__, ...)                                 \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_WARN, __VA_ARGS__)

#define PRW_LOG_ERROR(filterType__, ...)                                \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_ERROR, __VA_ARGS__)

#define PRW_LOG_FATAL(filterType__, ...)                                \
    PRW_LOG_MESSAGE_BODY(filterType__, LOG_LEVEL_FATAL, __VA_ARGS__)

#endif //PROJECTWAAAGH_LOG_HPP
