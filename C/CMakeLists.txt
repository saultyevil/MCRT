cmake_minimum_required(VERSION 3.10)
project(mcrt)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

include_directories(src)
include_directories(C:/GSL/include)

add_executable(mcrt
        src/histogram.c
        src/moments.c
        src/main.c
        src/functions.h
        src/variables.h
        src/random.c
        src/parameters.c
        src/time.c
        src/transport.c
        src/utilities.c
        src/write_file.c
)


find_package(GSL REQUIRED)
target_link_libraries(mcrt GSL::gsl GSL::gslcblas)
