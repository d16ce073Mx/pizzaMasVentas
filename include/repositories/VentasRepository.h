#pragma once
#include "crow.h"
#include "config/Config.h"
#include "services/PdfService.h"
#include <string>
#include <vector>

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

        crow::json::wvalue obtenerProductosVenta(
            long long orgId,
            long long productoId,            
            const std::string& tipoProducto
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

        int obtenerDatosPdfRecibo(
            long long reciboId,
            long long orgId,
            std::string& folio,
            std::string& fechaHora,
            std::string& tipoConsumo,
            std::vector<LineaPdf>& lineas,
            double& subtotal,
            double& descuento,
            double& impuesto,
            double& total,
            std::string& estatus
        );

        int pagarRecibo(
            long long reciboId,
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

        int finalizarRecibo(
            const std::string& folio,
            long long orgId,
            long long updatedBy,
            crow::json::wvalue& respuesta
        );

    };




}