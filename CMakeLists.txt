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

message("Platform = ${CMAKE_SYSTEM_NAME}")

IF (MSYS)
    add_custom_command(TARGET FPACker POST_BUILD
        COMMAND cp /usr/bin/msys-2.0.dll msys-2.0.dll
    )
ENDIF ()