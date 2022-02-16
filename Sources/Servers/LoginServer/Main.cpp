#include "Defines.hpp"
#include "ConfigManager.hpp"
#include "MySQLConnection.hpp"
#include "Banner.hpp"
#include "GitRevision.hpp"
#include "Utilities.hpp"
#include <boost/asio/signal_set.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <csignal>

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


    ProjectWaaagh::Banner::Show("LoginServer",
        [](char const* text) {
            printf("Server.LoginServer - %s\n", text);
        },
        []() {
            printf("Server.LoginServer - Using Configuration file %s\n", sConfigManager->GetFilename().c_str());
            printf("Server.LoginServer - Using SSL Version: X (library: X)\n");
            printf("Server.LoginServer - Using Boost Version: %i.%i.%i\n", BOOST_VERSION / 100000, BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
        }
    );

    //std::cout << sConfigManager->GetFilename().c_str() << std::endl;

    //std::cout << sConfigManager->GetStringDefault("LoginDatabaseInfo", "defaultValue", false) << std::endl;

    return 0;
}


bool StartDatabase() {
    return true;

}


void StopDatabase() {}