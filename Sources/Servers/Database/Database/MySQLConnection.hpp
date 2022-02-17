#ifndef PROJECTWAAAGH_MYSQLCONNECTION_HPP
#define PROJECTWAAAGH_MYSQLCONNECTION_HPP

#include "Defines.hpp"
#include "MySQLHacks.hpp"
#include "MySQLWorkaround.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

/***
 * MySQL Connection Flags
 */
enum ConnectionFlags {
    CONNECTION_ASYNC = 0x01,
    CONNECTION_SYNCH = 0x02,
    CONNECTION_BOTH = CONNECTION_ASYNC | CONNECTION_SYNCH
};

/***
 * ProjectWaaagh config string for mysql connection
 */
struct PRW_DATABASE_API MySQLConnectionInfo {
    explicit MySQLConnectionInfo(std::string const& infoString);

    std::string Username;
    std::string Password;
    std::string Hostname;
    std::string Database;
    std::string PortSocket;
    std::string SSL;
};

class PRW_DATABASE_API MySQLConnection {
public:
    // Constructor for synchronous connections
    MySQLConnection(MySQLConnectionInfo& connectionInfo);

    virtual ~MySQLConnection();

    virtual uint32 Open();
    void Close();

    bool PrepareStatements();

    bool Execute(char const* sqlString);

    void Ping();

protected:
    uint32 GetServerVersion() const;
    bool mB_Reconnecting;
    bool mB_PrepareError;
private:
    bool mB_HandleMySQLErrno(uint32 errNo, uint8 attempts = 5);

    MySQLHandle* mMysql;
    MySQLConnectionInfo& mConnectionInfo;
    ConnectionFlags mConnectionFlags;
    std::mutex mMutex;

    MySQLConnection(MySQLConnection const& right) = delete;
    MySQLConnection& operator=(MySQLConnection const& right) = delete;


};

#endif //PROJECTWAAAGH_MYSQLCONNECTION_HPP