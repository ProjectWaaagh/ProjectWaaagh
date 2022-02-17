#include "Defines.hpp"
#include "ConfigManager.hpp"
#include "MySQLConnection.hpp"
#include "Banner.hpp"
#include "GitRevision.hpp"
#include "Utilities.hpp"
#include "Log.hpp"
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <csignal>

#include <openssl/opensslv.h>
#include <openssl/crypto.h>

bool StartDatabase();
void StopDatabase();

#ifndef _PROJECTWAAAGH_LOGINSERVER_CONFIG
#   define _PROJECTWAAAGH_LOGINSERVER_CONFIG "LoginServer.conf"
#endif



int main(int argc, char** argv) { 

    auto configFile = boost::filesystem::absolute(_PROJECTWAAAGH_LOGINSERVER_CONFIG);
    std::string configError;
    if (!sConfigManager->Initialize(configFile.generic_string(),
        std::vector<std::string>(argv, argv + argc),
        configError)) {
        printf("Error in config file: '%s'\n", configError.c_str());
        return 1;
    }


    sLog->Initialize(nullptr);


    ProjectWaaagh::Banner::Show("LoginServer",
        [](char const* text) {
            printf("Server.LoginServer - %s\n", text);
        },
        []() {
            PRW_LOG_INFO("Server.LoginServer", "Using configuration file %s.", sConfigManager->GetFilename().c_str());
            PRW_LOG_INFO("Server.LoginServer", "Using SSL Version: %s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
            PRW_LOG_INFO("Server.LoginServer", "Using Boost Version: %i.%i.%i", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
        }

    );

    //std::cout << sConfigManager->GetFilename().c_str() << std::endl;

    //std::cout << sConfigManager->GetStringDefault("LoginDatabaseInfo", "defaultValue", false) << std::endl;

    PRW_LOG_INFO("Loginfo %s", sConfigManager->GetFilename());

    return 0;
}


bool StartDatabase() {
    return true;

}


void StopDatabase() {}