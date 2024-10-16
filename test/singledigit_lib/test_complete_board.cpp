//
// Created by Obi Davis on 11/09/2024.
//

#include "gtest/gtest.h"
#include "generator.hpp"
#include "board/board.hpp"

TEST(TestCompleteBoard, Valid) {
    puzzle_generator generator(0);
    auto board_str = generator.generate_solution();
    ASSERT_TRUE(board{board_str}.is_valid());
}

TEST(TestCompleteBoard, Unique) {
    puzzle_generator generator0(0);
    auto board0 = generator0.generate_solution();
    puzzle_generator generator1(0);
    auto board1 = generator1.generate_solution();
    ASSERT_EQ(board0, board1);

    puzzle_generator generator2(1);
    auto board2 = generator2.generate_solution();
    ASSERT_NE(board1, board2);
}
