#include "services/QrService.h"

#include <qrencode.h>
#include <png.h>

#include <cstdio>

namespace pizzaMas
{
    bool QrService::generarQr(
        const std::string& contenido,
        const std::string& rutaArchivo)
    {
        if (contenido.empty() || rutaArchivo.empty())
        {
            return false;
        }

        QRcode* qr = QRcode_encodeString(
            contenido.c_str(),
            0,
            QR_ECLEVEL_M,
            QR_MODE_8,
            1
        );

        if (qr == nullptr)
        {
            return false;
        }

        const int ancho = qr->width;
        const int escala = 10;
        const int margen = 4;

        const int imagenSize =
            (ancho + margen * 2) * escala;

        FILE* archivo = fopen(
            rutaArchivo.c_str(),
            "wb"
        );

        if (archivo == nullptr)
        {
            QRcode_free(qr);
            return false;
        }

        png_structp png =
            png_create_write_struct(
                PNG_LIBPNG_VER_STRING,
                nullptr,
                nullptr,
                nullptr
            );

        if (png == nullptr)
        {
            fclose(archivo);
            QRcode_free(qr);
            return false;
        }

        png_infop info =
            png_create_info_struct(png);

        if (info == nullptr)
        {
            png_destroy_write_struct(
                &png,
                nullptr
            );

            fclose(archivo);
            QRcode_free(qr);
            return false;
        }

        if (setjmp(png_jmpbuf(png)))
        {
            png_destroy_write_struct(
                &png,
                &info
            );

            fclose(archivo);
            QRcode_free(qr);
            return false;
        }

        png_init_io(
            png,
            archivo
        );

        png_set_IHDR(
            png,
            info,
            imagenSize,
            imagenSize,
            8,
            PNG_COLOR_TYPE_GRAY,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );

        png_write_info(
            png,
            info
        );

        std::vector<unsigned char> fila(
            imagenSize
        );

        for (int y = 0; y < imagenSize; ++y)
        {
            const int qrY =
                y / escala - margen;

            for (int x = 0; x < imagenSize; ++x)
            {
                const int qrX =
                    x / escala - margen;

                bool negro = false;

                if (qrX >= 0 &&
                    qrX < ancho &&
                    qrY >= 0 &&
                    qrY < ancho)
                {
                    const unsigned char valor =
                        qr->data[
                            qrY * ancho + qrX
                        ];

                    negro =
                        (valor & 1) != 0;
                }

                fila[x] =
                    negro ? 0 : 255;
            }

            png_write_row(
                png,
                fila.data()
            );
        }

        png_write_end(
            png,
            nullptr
        );

        png_destroy_write_struct(
            &png,
            &info
        );

        fclose(archivo);

        QRcode_free(qr);

        return true;
    }


    bool QrService::generarQrDatos(
        const std::string& contenido,
        std::vector<unsigned char>& datos,
        int& ancho,
        int& alto)
    {
        if (contenido.empty())
        {
            return false;
        }

        QRcode* qr = QRcode_encodeString(
            contenido.c_str(),
            0,
            QR_ECLEVEL_M,
            QR_MODE_8,
            1
        );

        if (qr == nullptr)
        {
            return false;
        }

        ancho = qr->width;
        alto = qr->width;

        const int margen = 4;

        const int anchoFinal =
            ancho + (margen * 2);

        const int altoFinal =
            alto + (margen * 2);

        const int bytesPorFila =
            (anchoFinal + 7) / 8;

        datos.assign(
            bytesPorFila * altoFinal,
            0
        );

        for (int y = 0; y < altoFinal; ++y)
        {
            const int qrY =
                y - margen;

            for (int x = 0; x < anchoFinal; ++x)
            {
                const int qrX =
                    x - margen;

                bool negro = false;

                if (qrX >= 0 &&
                    qrX < ancho &&
                    qrY >= 0 &&
                    qrY < alto)
                {
                    const unsigned char valor =
                        qr->data[
                            qrY * ancho + qrX
                        ];

                    negro =
                        (valor & 1) != 0;
                }

                if (negro)
                {
                    const int indiceByte =
                        y * bytesPorFila + (x / 8);

                    const int indiceBit =
                        7 - (x % 8);

                    datos[indiceByte] |=
                        static_cast<unsigned char>(
                            1 << indiceBit
                        );
                }
            }
        }

        ancho = anchoFinal;
        alto = altoFinal;

        QRcode_free(qr);

        return true;
    }
}