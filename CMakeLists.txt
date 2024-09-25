cmake_minimum_required(VERSION 3.25)
project(sudoku_gen)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()