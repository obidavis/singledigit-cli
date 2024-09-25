#include <solver.hpp>
#include <uniqueness.hpp>

#include "gtest/gtest.h"
#include "generator.hpp"
#include "board/board.hpp"


TEST(TestGenerate, Valid) {
    std::string solution = generate_complete_board(10);
    puzzle_generator gen(10);
    for (int i = 0; i < 25; ++i) {
        std::string puzzle = gen.generate_puzzle(solution);
        board bd(puzzle);
        EXPECT_TRUE(bd.is_valid());
        std::vector path = solve(bd, all_strategies);
        // if (path.empty()) {
        //     std::string message = "Puzzle is not solvable:\n" + puzzle;
        //     FAIL() << message;
        // }
        EXPECT_TRUE(is_unique(puzzle));
    }
}