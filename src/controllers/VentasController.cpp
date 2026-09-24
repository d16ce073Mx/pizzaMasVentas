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