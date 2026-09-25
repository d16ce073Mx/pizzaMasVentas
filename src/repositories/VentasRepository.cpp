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

    crow::json::wvalue VentasRepository::obtenerProductosVenta()
    {
        crow::json::wvalue respuesta;
        crow::json::wvalue::list productos;

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
            respuesta["error"] =
                "No fue posible conectar con PostgreSQL";

            PQfinish(connection);

            return respuesta;
        }

        const char* sql = R"SQL(
            SELECT
                p.pm_producto_id,
                p.pm_producto_codigo,
                p.pm_producto_nombre,
                p.pm_producto_descripcion,
                p.pm_producto_ventas_tipo,
                l.pm_precio_venta,
                lp.pm_org_id
            FROM pm_productos p
            INNER JOIN pm_listas_precios_lineas l
                ON l.pm_producto_id = p.pm_producto_id
            INNER JOIN pm_listas_precios lp
                ON lp.pm_lista_precio_id = l.pm_lista_precio_id
            WHERE p.pm_producto_ventas = 'VENTAS'
              AND lp.pm_org_id = 1
              AND p.pm_fecha_fin IS NULL
              AND lp.pm_fecha_fin IS NULL
              AND l.pm_fecha_fin IS NULL
            ORDER BY
                p.pm_producto_nombre;
        )SQL";

        PGresult* result = PQexec(connection, sql);

        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            respuesta["error"] =
                "Error al consultar los productos de venta";

            PQclear(result);
            PQfinish(connection);

            return respuesta;
        }

        for (int row = 0; row < PQntuples(result); ++row)
        {
            crow::json::wvalue producto;

            producto["id"] =
                std::stoi(PQgetvalue(result, row, 0));

            producto["codigo"] =
                PQgetvalue(result, row, 1);

            producto["nombre"] =
                PQgetvalue(result, row, 2);

            producto["descripcion"] =
                PQgetvalue(result, row, 3);

            producto["tipo"] =
                PQgetvalue(result, row, 4);

            producto["precio"] =
                std::stod(PQgetvalue(result, row, 5));

            producto["org_id"] =
                std::stoi(PQgetvalue(result, row, 6));

            productos.push_back(std::move(producto));
        }

        respuesta["productos"] =
            std::move(productos);

        PQclear(result);
        PQfinish(connection);

        return respuesta;
    }
}