#pragma once
#include "crow.h"
#include "config/Config.h"

#include <string>

namespace pizzaMas
{
    class VentasRepository
    {
    private:
        Config config;

    public:
        explicit VentasRepository(const Config& config);

        std::string obtenerMensaje();
        bool probarConexion();

        crow::json::wvalue obtenerProductosVenta();

        crow::json::wvalue obtenerProductoVenta(
            long long productoId,
            long long orgId
        );

        crow::json::wvalue crearRecibo(
            long long orgId,
            const std::string& tipoConsumo,
            long long createdBy
        );

        int crearLineaRecibo(
            long long reciboId,
            long long productoId,
            double cantidad,
            long long createdBy,
            crow::json::wvalue& respuesta
        );

    };
}