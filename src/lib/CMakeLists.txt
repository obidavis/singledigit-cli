add_library(sudokugen_lib
        board/board.cpp
        board/cell.cpp
        board/constraint_set.cpp
        board/value_set.cpp
        strategies/basic.cpp
        strategies/cell_combination.cpp
        strategies/intersection_removal.cpp
        strategies/value_combination.cpp
        solver.cpp
        generator.cpp
        board/cell_set.cpp
        grader.cpp)
target_include_directories(sudokugen_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
