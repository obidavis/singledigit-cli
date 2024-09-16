#include <solver.hpp>

#include "gtest/gtest.h"
#include "generator.hpp"
#include "board/board.hpp"


TEST(TestGenerate, Valid) {
    std::string solution = generate_complete_board(10);
    puzzle_generator gen(10);
    for (int i = 0; i < 100; ++i) {
        std::string puzzle = gen.generate(solution);
        board bd(puzzle);
        EXPECT_TRUE(bd.is_valid());
        std::vector path = solve(bd, all_strategies);
        EXPECT_TRUE(!path.empty());
    }
}