cmake_minimum_required(VERSION 3.27)
project(FPACker C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(FPACker
        common.c
        common.h
        main.c
        unpacker.c
        unpacker.h
        util.c
        util.h)
