cmake_minimum_required(VERSION 3.5...3.28)
project(Luna3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build examples programs" ON)

if(WIN32)
    option(DIRECT3D11 "Use the direct3d 11 the backend" ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(src)

add_library(Luna-Libraries INTERFACE)
add_library(Luna::Libraries ALIAS Luna-Libraries)
target_link_libraries(Luna-Libraries INTERFACE engine window graphics)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()