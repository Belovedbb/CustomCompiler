cmake_minimum_required(VERSION 3.8)

project(CustomCompiler)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(warnings "-Wall -Wextra -Werror")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_INSTALL_PREFIX {PROJECT_SOURCE_DIR})


set(INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/build)
set(INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INSTALL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)
