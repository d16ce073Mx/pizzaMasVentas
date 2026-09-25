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

}