#pragma once

#include <string>

namespace pizzaMas
{
    struct Config
    {
        std::string host;
        int port;

        std::string dbHost;
        int dbPort;
        std::string dbName;
        std::string dbUser;
        std::string dbPassword;

        static Config fromEnvironment();
    };
}