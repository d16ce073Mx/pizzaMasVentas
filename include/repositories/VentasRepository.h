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

    };
}