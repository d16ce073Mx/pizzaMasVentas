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

    crow::json::wvalue VentasRepository::obtenerProductoVenta(
        long long productoId,
        long long orgId)
        {
            crow::json::wvalue respuesta;

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
                WHERE p.pm_producto_id = $1
                AND p.pm_producto_ventas = 'VENTAS'
                AND lp.pm_org_id = $2
                AND p.pm_fecha_fin IS NULL
                AND lp.pm_fecha_fin IS NULL
                AND l.pm_fecha_fin IS NULL;
            )SQL";

            std::string productoIdStr =
                std::to_string(productoId);

            std::string orgIdStr =
                std::to_string(orgId);

            const char* params[2] = {
                productoIdStr.c_str(),
                orgIdStr.c_str()
            };

            PGresult* result =
                PQexecParams(
                    connection,
                    sql,
                    2,
                    nullptr,
                    params,
                    nullptr,
                    nullptr,
                    0
                );

            if (PQresultStatus(result) != PGRES_TUPLES_OK)
            {
                respuesta["error"] =
                    "Error al consultar el producto de venta";

                PQclear(result);
                PQfinish(connection);

                return respuesta;
            }

            if (PQntuples(result) == 0)
            {
                respuesta["error"] =
                    "Producto de venta no encontrado";

                PQclear(result);
                PQfinish(connection);

                return respuesta;
            }

            respuesta["id"] =
                std::stoll(PQgetvalue(result, 0, 0));

            respuesta["codigo"] =
                PQgetvalue(result, 0, 1);

            respuesta["nombre"] =
                PQgetvalue(result, 0, 2);

            respuesta["descripcion"] =
                PQgetvalue(result, 0, 3);

            respuesta["tipo"] =
                PQgetvalue(result, 0, 4);

            respuesta["precio"] =
                std::stod(PQgetvalue(result, 0, 5));

            respuesta["org_id"] =
                std::stoll(PQgetvalue(result, 0, 6));

            PQclear(result);
            PQfinish(connection);

            return respuesta;
        }

    crow::json::wvalue VentasRepository::crearRecibo(
        long long orgId,
        const std::string& tipoConsumo,
        long long createdBy)
    {
        crow::json::wvalue respuesta;

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
            INSERT INTO public.pm_recibos_ventas (
                pm_org_id,
                pm_recibo_venta,
                pm_recibo_tipo_consumo,
                created_by
            )
            VALUES (
                $1::INTEGER,
                fn_generar_recibo_folio($1),
                $2,
                $3
            )
            RETURNING
                pm_recibo_id,
                pm_org_id,
                pm_recibo_venta,
                pm_recibo_fecha_hora,
                pm_recibo_tipo_consumo,
                pm_recibo_subtotal,
                pm_recibo_descuento,
                pm_recibo_impuesto,
                pm_recibo_total,
                pm_recibo_estatus;
        )SQL";

        std::string orgIdStr =
            std::to_string(orgId);

        std::string createdByStr =
            std::to_string(createdBy);

        const char* params[3] = {
            orgIdStr.c_str(),
            tipoConsumo.c_str(),
            createdByStr.c_str()
        };

        PGresult* result =
            PQexecParams(
                connection,
                sql,
                3,
                nullptr,
                params,
                nullptr,
                nullptr,
                0
            );

        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            respuesta["error"] =
                PQerrorMessage(connection);

            PQclear(result);
            PQfinish(connection);

            return respuesta;
        }

        respuesta["id"] =
            std::stoll(PQgetvalue(result, 0, 0));

        respuesta["org_id"] =
            std::stoll(PQgetvalue(result, 0, 1));

        respuesta["folio"] =
            PQgetvalue(result, 0, 2);

        respuesta["fecha_hora"] =
            PQgetvalue(result, 0, 3);

        respuesta["tipo_consumo"] =
            PQgetvalue(result, 0, 4);

        respuesta["subtotal"] =
            std::stod(PQgetvalue(result, 0, 5));

        respuesta["descuento"] =
            std::stod(PQgetvalue(result, 0, 6));

        respuesta["impuesto"] =
            std::stod(PQgetvalue(result, 0, 7));

        respuesta["total"] =
            std::stod(PQgetvalue(result, 0, 8));

        respuesta["estatus"] =
            PQgetvalue(result, 0, 9);

        PQclear(result);
        PQfinish(connection);

        return respuesta;
    }

    int VentasRepository::crearLineaRecibo(
        long long reciboId,
        long long productoId,
        double cantidad,
        long long createdBy,
        crow::json::wvalue& respuesta)
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
            respuesta["error"] =
                "No fue posible conectar con PostgreSQL";

            PQfinish(connection);

            return 500;
        }

        const char* sql = R"SQL(
            INSERT INTO public.pm_recibo_venta_lineas (
                pm_recibo_id,
                pm_recibo_venta_linea,
                pm_producto_id,
                pm_recibo_linea_cantidad,
                pm_recibo_linea_precio,
                pm_recibo_linea_total,
                created_by
            )
            SELECT
                $1::BIGINT,
                COALESCE(
                    MAX(l.pm_recibo_venta_linea),
                    0
                ) + 1,
                p.pm_producto_id,
                $3::NUMERIC,
                lpl.pm_precio_venta,
                ROUND(
                    ($3::NUMERIC * lpl.pm_precio_venta),
                    2
                ),
                $4::INTEGER
            FROM public.pm_recibos_ventas r
            INNER JOIN public.pm_productos p
                ON p.pm_producto_id = $2::BIGINT
            INNER JOIN public.pm_listas_precios lp
                ON lp.pm_org_id = r.pm_org_id
            AND lp.pm_fecha_fin IS NULL
            INNER JOIN public.pm_listas_precios_lineas lpl
                ON lpl.pm_lista_precio_id =
                lp.pm_lista_precio_id
            AND lpl.pm_producto_id =
                p.pm_producto_id
            AND lpl.pm_fecha_fin IS NULL
            LEFT JOIN public.pm_recibo_venta_lineas l
                ON l.pm_recibo_id = r.pm_recibo_id
            WHERE r.pm_recibo_id = $1::BIGINT
              AND r.pm_recibo_estatus = 'ABIERTO'
              AND p.pm_producto_ventas = 'VENTAS'
              AND p.pm_fecha_fin IS NULL
            GROUP BY
                p.pm_producto_id,
                lpl.pm_precio_venta
            RETURNING
                pm_recibo_linea_id,
                pm_recibo_id,
                pm_recibo_venta_linea,
                pm_producto_id,
                pm_recibo_linea_cantidad,
                pm_recibo_linea_precio,
                pm_recibo_linea_total,
                created_by;
        )SQL";

        std::string reciboIdStr =
            std::to_string(reciboId);

        std::string productoIdStr =
            std::to_string(productoId);

        std::string cantidadStr =
            std::to_string(cantidad);

        std::string createdByStr =
            std::to_string(createdBy);

        const char* params[4] = {
            reciboIdStr.c_str(),
            productoIdStr.c_str(),
            cantidadStr.c_str(),
            createdByStr.c_str()
        };

        PGresult* result =
            PQexecParams(
                connection,
                sql,
                4,
                nullptr,
                params,
                nullptr,
                nullptr,
                0
            );

        if (PQresultStatus(result) != PGRES_TUPLES_OK)
        {
            respuesta["error"] =
                PQerrorMessage(connection);

            PQclear(result);
            PQfinish(connection);

            return 500;
        }

        if (PQntuples(result) == 0)
        {
            respuesta["error"] =
                "El recibo no esta abierto o no es posible agregar el producto";

            PQclear(result);
            PQfinish(connection);

            return 409;
        }

        respuesta["id"] =
            std::stoll(PQgetvalue(result, 0, 0));

        respuesta["recibo_id"] =
            std::stoll(PQgetvalue(result, 0, 1));

        respuesta["linea"] =
            std::stoi(PQgetvalue(result, 0, 2));

        respuesta["producto_id"] =
            std::stoll(PQgetvalue(result, 0, 3));

        respuesta["cantidad"] =
            std::stod(PQgetvalue(result, 0, 4));

        respuesta["precio"] =
            std::stod(PQgetvalue(result, 0, 5));

        respuesta["total"] =
            std::stod(PQgetvalue(result, 0, 6));

        respuesta["created_by"] =
            std::stoll(PQgetvalue(result, 0, 7));

        PQclear(result);
        PQfinish(connection);

        return 201;
    }

}