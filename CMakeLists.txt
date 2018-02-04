cmake_minimum_required(VERSION 3.6)
project(evx)

option(EVX_BUILD_EXAMPLES "build examples" OFF)
option(EVX_BUILD_TESTS "build tests" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/evx)

if(EVX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(EVX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
