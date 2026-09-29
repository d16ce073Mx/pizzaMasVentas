#pragma once

#include <string>
#include <vector>

namespace pizzaMas
{
    class QrService
    {
    public:

        static bool generarQr(
            const std::string& contenido,
            const std::string& rutaArchivo
        );

        static bool generarQrDatos(
            const std::string& contenido,
            std::vector<unsigned char>& datos,
            int& ancho,
            int& alto
        );
    };
}