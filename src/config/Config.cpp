#include "config/Config.h"

#include <cstdlib>
#include <stdexcept>

namespace pizzaMas
{
    Config Config::fromEnvironment()
    {
        Config config;

        config.host = std::getenv("PIZZAMAS_HOST")
            ? std::getenv("PIZZAMAS_HOST")
            : "0.0.0.0";

        config.port = std::getenv("PIZZAMAS_PORT")
            ? std::stoi(std::getenv("PIZZAMAS_PORT"))
            : 8081;

        config.dbHost = std::getenv("PIZZAMAS_DB_HOST")
            ? std::getenv("PIZZAMAS_DB_HOST")
            : "localhost";

        config.dbPort = std::getenv("PIZZAMAS_DB_PORT")
            ? std::stoi(std::getenv("PIZZAMAS_DB_PORT"))
            : 5433;

        config.dbName = std::getenv("PIZZAMAS_DB_NAME")
            ? std::getenv("PIZZAMAS_DB_NAME")
            : "pizzamas";

        config.dbUser = std::getenv("PIZZAMAS_DB_USER")
            ? std::getenv("PIZZAMAS_DB_USER")
            : "pizzamas";

        config.dbPassword = std::getenv("PIZZAMAS_DB_PASSWORD")
            ? std::getenv("PIZZAMAS_DB_PASSWORD")
            : "Pizzam45";

        return config;
    }
}