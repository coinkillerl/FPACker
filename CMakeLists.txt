cmake_minimum_required(VERSION 3.27)
project(FPACker C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build : using asan")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
ENDIF()

add_executable(FPACker
        common.c
        common.h
        main.c
        unpacker.c
        unpacker.h
        util.c
        util.h
        vector.h
        packer.h
        packer.c
        crc32.h)

message("Platform = ${CMAKE_SYSTEM_NAME}")

IF (MSYS)
    add_custom_command(TARGET FPACker POST_BUILD
        COMMAND cp /usr/bin/msys-2.0.dll msys-2.0.dll
    )
ENDIF ()
