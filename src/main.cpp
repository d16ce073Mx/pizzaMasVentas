#include "crow.h"
#include "config/Config.h"
#include "controllers/VentasController.h"
#include "services/VentasService.h"
#include "repositories/VentasRepository.h"
#include "config/DotEnv.h"
#include <iostream>

int main()
{
    pizzaMas::loadDotEnv(".env");
    const auto config = pizzaMas::Config::fromEnvironment();
    crow::SimpleApp app;

    pizzaMas::VentasRepository ventasRepository(config);

    pizzaMas::VentasService ventasService(ventasRepository);

    pizzaMas::VentasController::registerRoutes(
        app,
        ventasService
    );

    app.port(config.port);

    std::cout << "PizzaMas - Microservicio de Ventas" << std::endl;
    std::cout << "Host: " << config.host << std::endl;
    std::cout << "Puerto: " << config.port << std::endl;
    std::cout << "Base de datos: " << config.dbName << std::endl;

    app.run();

    return 0;
}