cmake_minimum_required(VERSION 3.8)

project(CompilerSource C)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES main.c chunk.c memory.c debug.c value.c vm.c compiler.c scanner.c)

add_executable(CompilerSource ${SOURCES})

install(TARGETS CompilerSource DESTINATION ${INSTALL_BIN_DIR})