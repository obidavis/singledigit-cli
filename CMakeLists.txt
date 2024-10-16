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

OPTION(BUILD_BENCHMARKS "Build the benchmarks" OFF)
if (BUILD_BENCHMARKS)
    set(GOOGLETEST_PATH "${CMAKE_CURRENT_LIST_DIR}/test/googletest" CACHE STRING "Path to Google Test source")
    add_subdirectory(bench)
endif ()