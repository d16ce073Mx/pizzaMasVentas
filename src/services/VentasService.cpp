#include "services/VentasService.h"
#include "services/PdfService.h"
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

    crow::json::wvalue VentasService::obtenerProductoVenta(
        long long productoId,
        long long orgId)
    {
        return repository.obtenerProductoVenta(
            productoId,
            orgId
        );
    }

    crow::json::wvalue VentasService::crearRecibo(
        long long orgId,
        const std::string& tipoConsumo,
        long long createdBy)
    {
        return repository.crearRecibo(
            orgId,
            tipoConsumo,
            createdBy
        );
    }

    int VentasService::pagarRecibo(
        long long reciboId,
        long long updatedBy,
        crow::json::wvalue& respuesta)
    {
        return repository.pagarRecibo(
            reciboId,
            updatedBy,
            respuesta
        );
    }

    crow::json::wvalue VentasService::obtenerRecibo(
        long long reciboId)
    {
        return repository.obtenerRecibo(
            reciboId
        );
    }

    int VentasService::crearLineaRecibo(
        long long reciboId,
        long long productoId,
        double cantidad,
        long long createdBy,
        crow::json::wvalue& respuesta)
    {
        return repository.crearLineaRecibo(
            reciboId,
            productoId,
            cantidad,
            createdBy,
            respuesta
        );
    }

    int VentasService::generarPdfRecibo(
        long long reciboId,
        const std::string& rutaArchivo)
    {
        std::string folio;
        std::string fechaHora;
        std::string tipoConsumo;
        std::vector<LineaPdf> lineas;

        double subtotal = 0.0;
        double descuento = 0.0;
        double impuesto = 0.0;
        double total = 0.0;

        std::string estatus;

        int resultado =
            repository.obtenerDatosPdfRecibo(
                reciboId,
                folio,
                fechaHora,
                tipoConsumo,
                lineas,
                subtotal,
                descuento,
                impuesto,
                total,
                estatus
            );

        if (resultado != 200)
        {
            return resultado;
        }

        bool pdfGenerado = 
            PdfService::generarPdf(
                rutaArchivo,
                folio,
                fechaHora,
                tipoConsumo,
                lineas,
                subtotal,
                descuento,
                impuesto,
                total,
                ""
            );

            return pdfGenerado ? 200 : 500;
    }

}