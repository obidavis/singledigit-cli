#include "gtest/gtest.h"
#include "generator/generator.hpp"
#include "board/board.hpp"
#include <chrono>

TEST(TestGenerate, Valid) {
    std::string solution = generate_complete_board(0);
    puzzle_generator gen(0);
    std::string puzzle = gen.generate(solution);
    board bd(puzzle);
    EXPECT_TRUE(bd.is_valid());
}