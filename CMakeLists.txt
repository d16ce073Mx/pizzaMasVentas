cmake_minimum_required(VERSION 3.20)

project(pizzaMasVentas
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Crow REQUIRED)

set(LIBPQ_ROOT "/usr/local/opt/libpq")

find_path(LIBPQ_INCLUDE_DIR
    NAMES libpq-fe.h
    PATHS "${LIBPQ_ROOT}/include"
    REQUIRED
)

find_library(LIBPQ_LIBRARY
    NAMES pq
    PATHS "${LIBPQ_ROOT}/lib"
    REQUIRED
)

add_executable(pizzaMasVentas
    src/main.cpp
    src/config/Config.cpp
    src/config/DotEnv.cpp
    src/controllers/VentasController.cpp
    src/services/VentasService.cpp
    src/repositories/VentasRepository.cpp
)

target_include_directories(pizzaMasVentas
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${LIBPQ_INCLUDE_DIR}
)

target_link_libraries(pizzaMasVentas
    PRIVATE
        Crow::Crow
        ${LIBPQ_LIBRARY}
)