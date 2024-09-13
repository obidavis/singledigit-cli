add_library(sudoku_gen
        board/board.cpp
        board/cell.cpp
        board/constraint_set.cpp
        board/value_set.cpp
        strategies/basic.cpp
        strategies/cell_combination.cpp
        strategies/intersection_removal.cpp
        strategies/value_combination.cpp
        strategies/elimination.cpp
        solver/solver.cpp
        generator/generator.cpp
        board/cell_set.cpp)
target_include_directories(sudoku_gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
