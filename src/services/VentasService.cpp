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

    bool VentasService::probarConexion()
    {
        return repository.probarConexion();
    }

}