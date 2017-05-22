cmake_minimum_required(VERSION 3.7)
project(MI11-TP-ARM)

set(CMAKE_C_STANDARD 11)

set(COMMON_FLAGS "-fno-builtin -ffunction-sections -fdata-sections -fno-strict-aliasing -fmessage-length=0")

## specify the cross compiler
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
## where is the target environment
execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-file-name=libc.a
        OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
SET(CMAKE_FIND_ROOT_PATH ${CMAKE_INSTALL_PREFIX})
## search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
## for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections --specs=nosys.specs -nostdlib -static -nostartfiles")

include_directories(Distribuable)

set(SOURCE_FILES testfile.c noyau.c
        Distribuable/imx_serial.c
#        Distribuable/noyau.c
        Distribuable/noyaufil.c
#        Distribuable/noyautes.c
        Distribuable/serialio.c)
add_executable(TESTFILE ${SOURCE_FILES})