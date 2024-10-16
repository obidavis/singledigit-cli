cmake_minimum_required(VERSION 3.25)
project(singledigit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

OPTION(BUILD_TESTS "Build the tests" OFF)
if (BUILD_TESTS)
    add_subdirectory(test)
    enable_testing()
endif ()