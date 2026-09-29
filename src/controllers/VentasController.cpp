#include "controllers/VentasController.h"
#include <fstream>
#include <cstdio>
namespace pizzaMas
{
    void VentasController::registerRoutes(
        crow::SimpleApp& app,
        VentasService& service)
    {
        CROW_ROUTE(app, "/api/ventas")
        ([&service] {
            return service.obtenerMensaje();
        });

        CROW_ROUTE(app, "/api/ventas/productos")
        ([&service] {
            return crow::response(
                service.obtenerProductosVenta()
            );
        });

        CROW_ROUTE(app, "/api/ventas/productos/<int>")
        ([&service](const crow::request& req, int productoId) {

            const char* orgIdParam =
                req.url_params.get("org_id");

            if (orgIdParam == nullptr)
            {
                return crow::response(
                    400,
                    "El parametro org_id es requerido"
                );
            }

            try
            {
                long long orgId =
                    std::stoll(orgIdParam);

                return crow::response(
                    service.obtenerProductoVenta(
                        productoId,
                        orgId
                    )
                );
            }
            catch (const std::exception&)
            {
                return crow::response(
                    400,
                    "El parametro org_id no es valido"
                );
            }
        });

        CROW_ROUTE(app, "/api/ventas/recibos")
        .methods(crow::HTTPMethod::POST)
        ([&service](const crow::request& req) {

            auto body =
                crow::json::load(req.body);

            if (!body)
            {
                return crow::response(
                    400,
                    "JSON invalido"
                );
            }

            if (!body.has("org_id") ||
                !body.has("tipo_consumo") ||
                !body.has("created_by"))
            {
                return crow::response(
                    400,
                    "Los parametros org_id, tipo_consumo y created_by son requeridos"
                );
            }

            try
            {
                long long orgId =
                    body["org_id"].i();

                std::string tipoConsumo =
                    body["tipo_consumo"].s();

                long long createdBy =
                    body["created_by"].i();

                return crow::response(
                    201,
                    service.crearRecibo(
                        orgId,
                        tipoConsumo,
                        createdBy
                    )
                );
            }
            catch (const std::exception&)
            {
                return crow::response(
                    400,
                    "Parametros invalidos"
                );
            }
        });

  
        CROW_ROUTE(app, "/api/ventas/recibos/<int>")
        ([&service](const crow::request& req, int reciboId) {

            auto respuesta =
                service.obtenerRecibo(reciboId);

            std::string json =
                respuesta.dump();

            return crow::response(
                200,
                json
            );
        });
    
        CROW_ROUTE(app, "/api/ventas/recibos/<int>/pagar")
        .methods(crow::HTTPMethod::POST)
        ([&service](const crow::request& req, int reciboId) {

            auto body =
                crow::json::load(req.body);

            if (!body)
            {
                return crow::response(
                    400,
                    "JSON invalido"
                );
            }

            if (!body.has("updated_by"))
            {
                return crow::response(
                    400,
                    "El parametro updated_by es requerido"
                );
            }

            try
            {
                long long updatedBy =
                    body["updated_by"].i();

                crow::json::wvalue respuesta;

                int status =
                    service.pagarRecibo(
                        reciboId,
                        updatedBy,
                        respuesta
                    );

                return crow::response(
                    status,
                    respuesta
                );
            }
            catch (const std::exception&)
            {
                return crow::response(
                    400,
                    "Parametros invalidos"
                );
            }
        });

        CROW_ROUTE(app, "/api/ventas/health/db")
        ([&service] {
            if (service.probarConexion())
            {
                return crow::response(200, "PostgreSQL: conexión exitosa");
            }

            return crow::response(500, "PostgreSQL: error de conexión");
        });

        CROW_ROUTE(app, "/api/ventas/recibos/<int>/lineas")
        .methods(crow::HTTPMethod::POST)
        ([&service](const crow::request& req, int reciboId) {

            auto body =
                crow::json::load(req.body);

            if (!body)
            {
                return crow::response(
                    400,
                    "JSON invalido"
                );
            }

            if (!body.has("producto_id") ||
                !body.has("cantidad") ||
                !body.has("created_by"))
            {
                return crow::response(
                    400,
                    "Los parametros producto_id, cantidad y created_by son requeridos"
                );
            }

            try
            {
                long long productoId =
                    body["producto_id"].i();

                double cantidad =
                    body["cantidad"].d();

                long long createdBy =
                    body["created_by"].i();

                if (cantidad <= 0)
                {
                    return crow::response(
                        400,
                        "La cantidad debe ser mayor que cero"
                    );
                }

                crow::json::wvalue respuesta;

                int status =
                    service.crearLineaRecibo(
                        reciboId,
                        productoId,
                        cantidad,
                        createdBy,
                        respuesta
                    );

                return crow::response(
                    status,
                    respuesta
                );
            }
            catch (const std::exception&)
            {
                return crow::response(
                    400,
                    "Parametros invalidos"
                );
            }
        });

        CROW_ROUTE(app, "/api/ventas/recibos/<int>/pdf")
        ([&service](const crow::request& req, int reciboId)
        {
            const std::string rutaArchivo =
                "recibo_" + std::to_string(reciboId) + ".pdf";

            int resultado =
                service.generarPdfRecibo(
                    reciboId,
                    rutaArchivo
                );

            if (resultado == 404)
            {
                return crow::response(
                    404,
                    "{\"error\":\"El recibo no existe.\"}"
                );
            }

            if (resultado == 409)
            {
                return crow::response(
                    409,
                    "{\"error\":\"El recibo no esta pagado\"}"
                );
            }

            if (resultado != 200)
            {
                return crow::response(
                    500,
                    "{\"error\":\"No fue posible generar el PDF del recibo\"}"
                );
            }

            crow::response respuesta(200);

            respuesta.set_header(
                "Content-Type",
                "application/pdf"
            );

            respuesta.set_header(
                "Content-Disposition",
                "inline; filename=\"" +
                rutaArchivo +
                "\""
            );

            std::ifstream archivo(rutaArchivo, std::ios::binary);

            if (!archivo)
            {
                return crow::response(
                    500,
                    "{\"error\":\"No fue posible abrir el archivo PDF\"}"
                );
            }

            std::string contenido(
                (std::istreambuf_iterator<char>(archivo)),
                std::istreambuf_iterator<char>()
            );

            respuesta.body = contenido;
            std::remove(rutaArchivo.c_str());
            return respuesta;
        });
    }
}