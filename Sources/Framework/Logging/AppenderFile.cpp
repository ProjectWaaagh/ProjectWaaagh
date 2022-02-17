#include "AppenderFile.hpp"
#include "Log.hpp"
#include "LogMessage.hpp"
#include "StringConverter.hpp"
#include "StringFormat.hpp"
#include "Utilities.hpp"

#include <algorithm>

AppenderFile::AppenderFile(uint8 id, const std::string &name, LogLevel level, AppenderFlags flags,
                           const std::vector<std::string_view> &args) : Appender(id, name, level, flags), logFile(
        nullptr), mLogDir(sLog->GetLogsDir()), mMaxFileSize(0), mFileSize(0) {

    if(args.size() < 4)
        throw InvalidAppenderArgsException(ProjectWaaagh::StringFormat("Log::CreateAppenderFromConfig: Missing file name or appender %s", name.c_str()));

    mFilename.assign(args[3]);

    std::string mode = "a";
    if(4 < args.size())
        mode.assign(args[4]);

    if (flags & APPENDER_FLAGS_USE_TIMESTAMP) {
        size_t dot_pos = mFilename.find_last_of('.');
        if(dot_pos != std::string::npos)
            mFilename.insert(dot_pos, sLog->GetLogsTimestamp());
        else
            mFilename += sLog->GetLogsTimestamp();
    }

    if(5 < args.size()) {
        if(Optional<uint32> size = ProjectWaaagh::StringTo<uint32>(args[5]))
            mMaxFileSize = *size;
        else
            throw InvalidAppenderArgsException(
                    ProjectWaaagh::StringFormat("Log::CreateAppenderFromConfig: Invalid size '%s' for appender %s",
                                                                           std::string(args[5]).c_str(), name.c_str()));
    }

    mDynamicName = std::string::npos != mFilename.find("%s");
    mBackup = (flags & APPENDER_FLAGS_MAKE_FILE_BACKUP) != 0;

    if(!mDynamicName)
        logFile = OpenFile(mFilename, mode, (mode == "w") && mBackup);
}

AppenderFile::~AppenderFile() {
    CloseFile();
}

void AppenderFile::_Write(const LogMessage *message) {
    bool exceedMaxSize = mMaxFileSize > 0 && (mFileSize.load() + message->Size()) > mMaxFileSize;

    if(mDynamicName) {
        char nameBuf[PROJECTWAAAGH_PATH_MAX];
        snprintf(nameBuf, PROJECTWAAAGH_PATH_MAX, mFilename.c_str(), message->param1.c_str());
        FILE* file = OpenFile(nameBuf, "a", mBackup || exceedMaxSize);
        if(!file)
            return;
        fprintf(file, "%s%s\n", message->prefix.c_str(), message->text.c_str());
        fflush(file);
        mFileSize += uint64(message->Size());
        fclose(file);
        return;
    } else if (exceedMaxSize)
        logFile = OpenFile(mFilename, "w", true);

    if(!logFile)
        return;

    fprintf(logFile, "%s%s\n", message->prefix.c_str(), message->text.c_str());
    fflush(logFile);
    mFilename += uint64(message->Size());
}

FILE* AppenderFile::OpenFile(const std::string &name, const std::string &mode, bool backup) {
    std::string fullName(mLogDir + name);
    if(backup) {
        CloseFile();
        std::string newName(fullName);
        newName.push_back('.');
        newName.append(LogMessage::GetTimeStr(time(nullptr)));
        std::replace(newName.begin(), newName.end(), ':', '-');
        rename(fullName.c_str(), newName.c_str());
    }

    if(FILE* ret = fopen(fullName.c_str(), mode.c_str())) {
        mFileSize = ftell(ret);
        return ret;
    }

    return nullptr;
}

void AppenderFile::CloseFile() {
    if(logFile) {
        fclose(logFile);
        logFile = nullptr;
    }
}

