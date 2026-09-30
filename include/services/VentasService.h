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

        crow::json::wvalue obtenerRecibo(
            long long reciboId,
            long long orgId
        );

        int pagarRecibo(
            long long reciboId,
            long long orgId,
            long long updatedBy,
            crow::json::wvalue& respuesta
        );

        int finalizarRecibo(
            const std::string& folio,
            long long orgId,
            long long updatedBy,
            crow::json::wvalue& respuesta
        );

        int crearLineaRecibo(
            long long reciboId,
            long long orgId,
            long long productoId,
            double cantidad,
            long long createdBy,
            crow::json::wvalue& respuesta
        );

        int generarPdfRecibo(
            long long reciboId,
            long long orgId,
            const std::string& rutaArchivo
        );
    };

    


}