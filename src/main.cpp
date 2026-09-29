#include <iostream>

#include "crow.h"

#include "config/Config.h"
#include "config/DotEnv.h"
#include "repositories/VentasRepository.h"
#include "services/VentasService.h"
#include "controllers/VentasController.h"

int main()
{
    std::cout
        << "Microservicio de Ventas PizzaMas"
        << std::endl;

    pizzaMas::loadDotEnv(".env");

    pizzaMas::Config config =
        pizzaMas::Config::fromEnvironment();

    pizzaMas::VentasRepository repository(config);

    pizzaMas::VentasService service(repository);

    crow::SimpleApp app;

    pizzaMas::VentasController::registerRoutes(
        app,
        service
    );

    std::cout
        << "Servidor iniciado en http://"
        << config.host
        << ":"
        << config.port
        << std::endl;

    app.port(config.port)
       .run();

    return 0;
}