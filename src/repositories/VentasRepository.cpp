#include "repositories/VentasRepository.h"

#include <libpq-fe.h>

namespace pizzaMas
{
    VentasRepository::VentasRepository(const Config& config)
        : config(config)
    {
    }

    std::string VentasRepository::obtenerMensaje()
    {
        return "PizzaMas - Repository de Ventas";
    }

    bool VentasRepository::probarConexion()
    {
        const std::string connectionString =
            "host=" + config.dbHost +
            " port=" + std::to_string(config.dbPort) +
            " dbname=" + config.dbName +
            " user=" + config.dbUser +
            " password=" + config.dbPassword;

        PGconn* connection =
            PQconnectdb(connectionString.c_str());

        if (PQstatus(connection) != CONNECTION_OK)
        {
            PQfinish(connection);
            return false;
        }

        PQfinish(connection);

        return true;
    }
}