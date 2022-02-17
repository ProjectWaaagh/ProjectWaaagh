#ifndef PROJECTWAAAGH_APPENDERFILE_HPP
#define PROJECTWAAAGH_APPENDERFILE_HPP

#include "Appender.hpp"
#include "SmartEnum.hpp"
#include <atomic>

class PRW_FRAMEWORK_API AppenderFile : public Appender {
public:
    static constexpr AppenderType type = APPENDER_FILE;

    AppenderFile(uint8 id, std::string const& name, LogLevel level, AppenderFlags flags, std::vector<std::string_view> const& args);
    ~AppenderFile();
    FILE* OpenFile(std::string const& name, std::string const& mode, bool backup);
    AppenderType GetType() const override { return type; };

private:
    void CloseFile();
    void _Write(LogMessage const* message) override;
    FILE* logFile;
    std::string mFilename;
    std::string mLogDir;
    bool mDynamicName;
    bool mBackup;
    uint64 mMaxFileSize;
    std::atomic<uint64> mFileSize;
};

#endif //PROJECTWAAAGH_APPENDERFILE_HPP