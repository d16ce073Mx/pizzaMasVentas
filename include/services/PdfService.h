#pragma once

#include <string>
#include <vector>

namespace pizzaMas
{
    struct LineaPdf
    {
        int linea;
        std::string producto;
        double cantidad;
        double precio;
        double total;
    };

    class PdfService
    {
    public:

        static bool generarPdf(
            const std::string& rutaArchivo,
            const std::string& folio,
            const std::string& fechaHora,
            const std::string& tipoConsumo,
            const std::vector<LineaPdf>& lineas,
            double subtotal,
            double descuento,
            double impuesto,
            double total,
            const std::string& rutaQr
        );
    };
}