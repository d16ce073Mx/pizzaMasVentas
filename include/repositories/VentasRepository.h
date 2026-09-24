#pragma once

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
    };
}