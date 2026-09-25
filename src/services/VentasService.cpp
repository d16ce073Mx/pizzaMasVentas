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

}