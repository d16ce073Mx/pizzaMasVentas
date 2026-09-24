#pragma once

#include "crow.h"
#include "services/VentasService.h"

namespace pizzaMas
{
    class VentasController
    {
    public:
        static void registerRoutes(
            crow::SimpleApp& app,
            VentasService& service);
    };
}