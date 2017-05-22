cmake_minimum_required(VERSION 3.7)
project(MI11-TP-ARM)

set(CMAKE_C_STANDARD 11)

## specify the cross compiler
#SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc -mfloat-abi=hard)
#SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++ -mfloat-abi=hard)
#set(CMAKE_AR arm-linux-gnueabihf-ar CACHE FILEPATH "Archiver")
#
## where is the target environment
#SET(CMAKE_FIND_ROOT_PATH /usr/local/linaro/arm-linux-gnueabihf/arm-linux-gnueabihf/libc)
#
## search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
## for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(Distribuable)

set(SOURCE_FILES testfile.c noyau.c Distribuable/noyaufil.c)
add_executable(TESTFILE ${SOURCE_FILES})