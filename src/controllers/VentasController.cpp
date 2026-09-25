#include "controllers/VentasController.h"

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


        CROW_ROUTE(app, "/api/ventas/health/db")
        ([&service] {
            if (service.probarConexion())
            {
                return crow::response(200, "PostgreSQL: conexión exitosa");
            }

            return crow::response(500, "PostgreSQL: error de conexión");
        });
    }
}