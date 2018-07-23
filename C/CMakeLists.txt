cmake_minimum_required(VERSION 3.10)
project(MCRT)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
include_directories(C:/GSL/include)

add_executable(
        MCRT
        src/default_params.c
        src/intensity.c
        src/moments.c
        src/plane.c
        src/plane_funcs.h
        src/plane_vars.h
        src/random.c
        src/read_par.c
        src/time.c
        src/transport.c
        src/utilities.c
        src/write_file.c
)


find_package(GSL REQUIRED)
target_link_libraries(MCRT GSL::gsl GSL::gslcblas)
