add_library(singledigit_lib
        board/board.cpp
        board/cell.cpp
        board/constraint_set.cpp
        board/value_set.cpp
        strategies/basic.cpp
        strategies/cell_combination.cpp
        strategies/intersection_removal.cpp
        strategies/value_combination.cpp
        strategies/fish.cpp
        solver.cpp
        generator.cpp
        board/cell_set.cpp
        grader.cpp
        uniqueness.cpp)
target_include_directories(singledigit_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(singledigit_lib PRIVATE fmt)
