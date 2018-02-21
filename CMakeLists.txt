cmake_minimum_required(VERSION 3.6)
project(cst/evx)

option(EVX_BUILD_EXAMPLES "build examples" OFF)
option(EVX_BUILD_TEST "build test" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Checking out dependent libraries...")
execute_process(COMMAND git submodule update --init --recursive
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(src/cst/logging)
add_subdirectory(src/cst/evx)

if(EVX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(EVX_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
