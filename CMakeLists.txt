cmake_minimum_required(VERSION 3.20)

project(pizzaMasVentas
    VERSION 1.0.0
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.3.4
)

FetchContent_MakeAvailable(Crow)

FetchContent_Declare(
    libharu
    GIT_REPOSITORY https://github.com/libharu/libharu.git
    GIT_TAG v2.4.6
)

FetchContent_MakeAvailable(libharu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



find_package(PNG REQUIRED)

find_path(LIBPQ_INCLUDE_DIR
    NAMES libpq-fe.h
    PATHS /usr/include/postgresql
    REQUIRED
)

set(QRENCODE_ROOT "/usr/local/opt/qrencode")

find_path(QRENCODE_INCLUDE_DIR
    NAMES qrencode.h
    PATHS /usr/include
    REQUIRED
)

find_library(QRENCODE_LIBRARY
    NAMES qrencode
    PATHS /usr/lib/aarch64-linux-gnu /usr/lib/x86_64-linux-gnu
    REQUIRED
)

find_library(LIBPQ_LIBRARY
    NAMES pq
    PATHS /usr/lib/aarch64-linux-gnu /usr/lib/x86_64-linux-gnu
    REQUIRED
)

add_executable(pizzaMasVentas
    src/main.cpp
    src/config/Config.cpp
    src/config/DotEnv.cpp
    src/controllers/VentasController.cpp
    src/services/VentasService.cpp
    src/repositories/VentasRepository.cpp
    src/services/QrService.cpp
    src/services/PdfService.cpp
)


target_include_directories(pizzaMasVentas
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${LIBPQ_INCLUDE_DIR}
        ${QRENCODE_INCLUDE_DIR}
        ${CMAKE_BINARY_DIR}/_deps/libharu-build/include
        ${CMAKE_BINARY_DIR}/_deps/libharu-src/include
)

target_link_libraries(pizzaMasVentas
    PRIVATE
        Crow::Crow
        ${LIBPQ_LIBRARY}
        ${QRENCODE_LIBRARY}
        PNG::PNG
        hpdf
)

add_dependencies(
    pizzaMasVentas
    hpdf
)