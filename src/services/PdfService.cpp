#include "services/PdfService.h"
#include "services/QrService.h"
#include <hpdf.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iostream>

namespace pizzaMas
{
    bool PdfService::generarPdf(
        const std::string& rutaArchivo,
        const std::string& folio,
        const std::string& fechaHora,
        const std::string& tipoConsumo,
        const std::vector<LineaPdf>& lineas,
        double subtotal,
        double descuento,
        double impuesto,
        double total,
        const std::string& rutaQr)
    {
        if (rutaArchivo.empty())
        {
            return false;
        }

        HPDF_Doc pdf = HPDF_New(
            nullptr,
            nullptr
        );

        if (pdf == nullptr)
        {
            return false;
        }

        HPDF_Page pagina =
            HPDF_AddPage(pdf);

        if (pagina == nullptr)
        {
            HPDF_Free(pdf);
            return false;
        }

        HPDF_Page_SetSize(
            pagina,
            HPDF_PAGE_SIZE_A4,
            HPDF_PAGE_PORTRAIT
        );

        HPDF_Font fuente =
            HPDF_GetFont(
                pdf,
                "Helvetica",
                nullptr
            );

        HPDF_Font fuenteNegrita =
            HPDF_GetFont(
                pdf,
                "Helvetica-Bold",
                nullptr
            );

        HPDF_Page_SetFontAndSize(
            pagina,
            fuenteNegrita,
            20
        );

        HPDF_Page_BeginText(pagina);

        HPDF_Page_TextOut(
            pagina,
            50,
            780,
            "PizzaMas"
        );

        HPDF_Page_EndText(pagina);

        HPDF_Page_SetFontAndSize(
            pagina,
            fuente,
            11
        );

        HPDF_Page_BeginText(pagina);

        HPDF_Page_TextOut(
            pagina,
            50,
            745,
            ("Folio: " + folio).c_str()
        );

        HPDF_Page_TextOut(
            pagina,
            50,
            725,
            ("Fecha: " + fechaHora).c_str()
        );

        HPDF_Page_TextOut(
            pagina,
            50,
            705,
            ("Tipo de consumo: " + tipoConsumo).c_str()
        );

        HPDF_Page_EndText(pagina);

        std::ostringstream texto;

        texto << std::fixed
            << std::setprecision(2);

        // Encabezado de productos
        HPDF_Page_BeginText(pagina);

        HPDF_Page_SetFontAndSize(
            pagina,
            fuenteNegrita,
            11
        );

        HPDF_Page_TextOut(
            pagina,
            50,
            675,
            "Productos"
        );

        HPDF_Page_EndText(pagina);

        // Líneas de productos
        double posicionY = 650;

        HPDF_Page_SetFontAndSize(
            pagina,
            fuente,
            10
        );

        for (const auto& linea : lineas)
        {
            HPDF_Page_BeginText(pagina);

            texto.str("");
            texto << linea.cantidad
                << " x "
                << linea.producto;

            HPDF_Page_TextOut(
                pagina,
                50,
                posicionY,
                texto.str().c_str()
            );

            texto.str("");
            texto << "$"
                << linea.total;

            HPDF_Page_TextOut(
                pagina,
                450,
                posicionY,
                texto.str().c_str()
            );

            HPDF_Page_EndText(pagina);

            posicionY -= 20;
        }

        // Totales
        posicionY -= 20;

        HPDF_Page_BeginText(pagina);

        texto.str("");
        texto << "Subtotal: $" << subtotal;

        HPDF_Page_TextOut(
            pagina,
            50,
            posicionY,
            texto.str().c_str()
        );

        posicionY -= 20;

        texto.str("");
        texto << "Descuento: $" << descuento;

        HPDF_Page_TextOut(
            pagina,
            50,
            posicionY,
            texto.str().c_str()
        );

        posicionY -= 20;

        texto.str("");
        texto << "Impuesto: $" << impuesto;

        HPDF_Page_TextOut(
            pagina,
            50,
            posicionY,
            texto.str().c_str()
        );

        posicionY -= 30;

        HPDF_Page_SetFontAndSize(
            pagina,
            fuenteNegrita,
            14
        );

        texto.str("");
        texto << "TOTAL: $" << total;

        HPDF_Page_TextOut(
            pagina,
            50,
            posicionY,
            texto.str().c_str()
        );

        HPDF_Page_EndText(pagina);

        if (!folio.empty())
        {
            std::vector<unsigned char> datosQr;

            int anchoQr = 0;
            int altoQr = 0;

            bool qrGenerado =
                QrService::generarQrDatos(
                    folio,
                    datosQr,
                    anchoQr,
                    altoQr
                );

            if (qrGenerado)
            {
                const int bytesPorFila =
                    (anchoQr + 7) / 8;

                HPDF_Image imagenQr =
                    HPDF_LoadRawImageFromMem(
                        pdf,
                        datosQr.data(),
                        anchoQr,
                        altoQr,
                        HPDF_CS_DEVICE_GRAY,
                        1
                    );

                if (imagenQr != nullptr)
                {
                    HPDF_Page_DrawImage(
                        pagina,
                        imagenQr,
                        400,
                        430,
                        120,
                        120
                    );
                }
            }
        }
        
        if (!rutaQr.empty())
        {
            std::cout
                << "Cargando QR: "
                << rutaQr
                << std::endl;

            HPDF_Image imagenQr =
                HPDF_LoadPngImageFromFile(
                    pdf,
                    rutaQr.c_str()
                );

            HPDF_STATUS errorQr =
                HPDF_GetError(pdf);

            HPDF_STATUS detalleQr =
                HPDF_GetErrorDetail(pdf);

            std::cout
                << "Error despues de cargar QR: "
                << errorQr
                << std::endl;

            std::cout
                << "Detalle QR: "
                << detalleQr
                << std::endl;

            if (imagenQr != nullptr)
            {
                std::cout
                    << "Imagen QR cargada correctamente"
                    << std::endl;

                HPDF_Page_DrawImage(
                    pagina,
                    imagenQr,
                    400,
                    430,
                    120,
                    120
                );
            }
            else
            {
                std::cout
                    << "No se pudo cargar la imagen QR"
                    << std::endl;
            }
        }

        /*
        HPDF_STATUS resultado =
            HPDF_SaveToFile(
                pdf,
                rutaArchivo.c_str()
            );
            */

        HPDF_STATUS errorAntesDeGuardar =
            HPDF_GetError(pdf);

        HPDF_STATUS detalleAntesDeGuardar =
            HPDF_GetErrorDetail(pdf);

        std::cout
            << "Error antes de guardar: "
            << errorAntesDeGuardar
            << std::endl;

        std::cout
            << "Detalle antes de guardar: "
            << detalleAntesDeGuardar
            << std::endl;

        HPDF_STATUS resultado =
            HPDF_SaveToFile(
                pdf,
                rutaArchivo.c_str()
            );    

        if (resultado != HPDF_OK)
        {
            std::cout
                << "Error al guardar PDF. Codigo libHaru: "
                << resultado
                << std::endl;
        }

        HPDF_Free(pdf);

        return resultado == HPDF_OK;
    }
}