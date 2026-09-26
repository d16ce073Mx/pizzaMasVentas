#pragma once

#include "repositories/VentasRepository.h"

#include <string>

namespace pizzaMas
{
    class VentasService
    {
    private:
        VentasRepository& repository;

    public:
        explicit VentasService(VentasRepository& repository);

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