#include "services/VentasService.h"

namespace pizzaMas
{
    VentasService::VentasService(VentasRepository& repository)
        : repository(repository)
    {
    }

    std::string VentasService::obtenerMensaje()
    {
        return repository.obtenerMensaje();
    }

    crow::json::wvalue VentasService::obtenerProductosVenta()
    {
        return repository.obtenerProductosVenta();
    }

    bool VentasService::probarConexion()
    {
        return repository.probarConexion();
    }

    crow::json::wvalue VentasService::obtenerProductoVenta(
        long long productoId,
        long long orgId)
    {
        return repository.obtenerProductoVenta(
            productoId,
            orgId
        );
    }

    crow::json::wvalue VentasService::crearRecibo(
        long long orgId,
        const std::string& tipoConsumo,
        long long createdBy)
    {
        return repository.crearRecibo(
            orgId,
            tipoConsumo,
            createdBy
        );
    }

    int VentasService::crearLineaRecibo(
        long long reciboId,
        long long productoId,
        double cantidad,
        long long createdBy,
        crow::json::wvalue& respuesta)
    {
        return repository.crearLineaRecibo(
            reciboId,
            productoId,
            cantidad,
            createdBy,
            respuesta
        );
    }

}