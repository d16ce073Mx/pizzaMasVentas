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
    };
}