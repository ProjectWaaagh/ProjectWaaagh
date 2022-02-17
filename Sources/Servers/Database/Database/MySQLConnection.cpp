#include "MySQLConnection.hpp"
#include "Framework.hpp"
#include "Timer.hpp"
#include "Utilities.hpp"
#include "MySQLHacks.hpp"
#include "MySQLWorkaround.hpp"
#include <errmsg.h>
#include <mysqld_error.h>


MySQLConnectionInfo::MySQLConnectionInfo(const std::string &infoString) {

    std::vector<std::string_view> tokens = ProjectWaaagh::Tokenize(infoString, ';', true);

    /**
     * Check if the config string right.
     */
    if(tokens.size() != 5 && tokens.size() != 6)
        return;

    Hostname.assign(tokens[0]);
    PortSocket.assign(tokens[1]);
    Username.assign(tokens[2]);
    Password.assign(tokens[3]);
    Database.assign(tokens[4]);

    if(tokens.size() == 6)
        SSL.assign(tokens[5]);
}

MySQLConnection::MySQLConnection(MySQLConnectionInfo & connectionInfo) :
        mB_Reconnecting(false), mB_PrepareError(false), mMysql(nullptr), mConnectionInfo(connectionInfo), mConnectionFlags(CONNECTION_SYNCH) {}


MySQLConnection::~MySQLConnection() {
    Close();
}

void MySQLConnection::Close() {
    if(mMysql) {
        mysql_close(mMysql);
        mMysql = nullptr;
    }
}

uint32 MySQLConnection::Open()
{
    MYSQL *mysqlInit;
    mysqlInit = mysql_init(nullptr);
    if (!mysqlInit)
    {
        printf("SQL.MySQL - Could not initialize Mysql connection to database `%s`", mConnectionInfo.Database.c_str());
        return CR_UNKNOWN_ERROR;
    }

    int port;
    char const* unix_socket;

    mysql_options(mysqlInit, MYSQL_SET_CHARSET_NAME, "utf8mb4");

#ifdef _WIN32
    if (mConnectionInfo.Hostname == ".") {
        unsigned int opt = MYSQL_PROTOCOL_PIPE;
        mysql_options(mysqlInit, MYSQL_OPT_PROTOCOL, (char const*)&opt);
        port = 0;
        unix_socket = nullptr;
    } else {
        port = atoi(mConnectionInfo.PortSocket.c_str());
        unix_socket = nullptr;
    }
#else
    if (mConnectionInfo.Hostname == ".") {
        unsigned int opt = MYSQL_PROTOCOL_SOCKET;
        mysql_options(mysqlInit, MYSQL_OPT_PROTOCOL, (char const*)&opt);
        mConnectionInfo.Hostname = "localhost";
        port = 0;
        unix_socket = mConnectionInfo.PortSocket.c_str();
    } else {
        port = atoi(mConnectionInfo.PortSocket.c_str());
        unix_socket = nullptr;
    }
#endif

    if (mConnectionInfo.SSL != "")
    {
#if !defined(MARIADB_VERSION_ID) && MYSQL_VERSION_ID >= 80000
        mysql_ssl_mode opt_use_ssl = SSL_MODE_DISABLED;
        if (mConnectionInfo.SSL == "ssl") {
            opt_use_ssl = SSL_MODE_REQUIRED;
        }
        mysql_options(mysqlInit, MYSQL_OPT_SSL_MODE, (char const*)&opt_use_ssl);
#else
        MySQLBool opt_use_ssl = MySQLBool(0);
        if (mConnectionInfo.SSL == "ssl") {
            opt_use_ssl = MySQLBool(1);
        }
        mysql_options(mysqlInit, MYSQL_OPT_SSL_ENFORCE, (char const*)&opt_use_ssl);
#endif
    }

    mMysql = reinterpret_cast<MySQLHandle*>(mysql_real_connect(mysqlInit, mConnectionInfo.Hostname.c_str(), mConnectionInfo.Username.c_str(),
                                                               mConnectionInfo.Password.c_str(), mConnectionInfo.Database.c_str(), port, unix_socket, 0));

    if (mMysql) {
        if (!mB_Reconnecting)
        {
            printf("SQL.MySQL - MySQL client library: %s", mysql_get_client_info());
            printf("SQL.MySQL - MySQL server ver: %s ", mysql_get_server_info(mMysql));
            // MySQL version above 5.1 IS required in both client and server and there is no known issue with different versions above 5.1
            // if (mysql_get_server_version(m_Mysql) != mysql_get_client_version())
            //     TC_LOG_INFO("sql.sql", "[WARNING] MySQL client/server version mismatch; may conflict with behaviour of prepared statements.");
        }

        printf("SQL.MySQL - Connected to MySQL database at %s", mConnectionInfo.Hostname.c_str());
        mysql_autocommit(mMysql, 1);

        // set connection properties to UTF8 to properly handle locales for different
        // server configs - core sends data in UTF8, so MySQL must expect UTF8 too
        mysql_set_character_set(mMysql, "utf8mb4");
        return 0;
    } else {
        printf("SQL.MySQL - Could not connect to MySQL database at %s: %s", mConnectionInfo.Hostname.c_str(), mysql_error(mysqlInit));
        uint32 errorCode = mysql_errno(mysqlInit);
        mysql_close(mysqlInit);
        return errorCode;
    }
}

bool MySQLConnection::PrepareStatements() {
    return false;
}

bool MySQLConnection::Execute(const char *sqlString) {
    if(!mMysql)
        return false;
    {
        uint32 _s = getMSTime();
        if(mysql_query(mMysql, sqlString)) {
            uint32 lErrno = mysql_errno(mMysql);
            printf("SQL.MySQL - SQL: %s", sqlString);
            printf("SQL.MySQL - [%u] %s", lErrno, mysql_error(mMysql));

            return false;
        } else {
            printf("SQL.MySQL - [%u ms] SQL: %s", getMSTimeDiff(_s, getMSTime()), sqlString);
        }
    }
    return true;
}

void MySQLConnection::Ping() {
    mysql_ping(mMysql);
}

uint32 MySQLConnection::GetServerVersion() const {
    return mysql_get_server_version(mMysql);
}