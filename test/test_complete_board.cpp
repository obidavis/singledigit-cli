//
// Created by Obi Davis on 11/09/2024.
//

#include "gtest/gtest.h"
#include "generator/generator.hpp"
#include "board/board.hpp"

TEST(TestCompleteBoard, Valid) {
    auto board_str = generate_complete_board(0);
    ASSERT_TRUE(board{board_str}.is_valid());
}

TEST(TestCompleteBoard, Unique) {
    auto board1 = generate_complete_board(0);
    auto board2 = generate_complete_board(0);
    ASSERT_EQ(board1, board2);

    auto board3 = generate_complete_board(1);
    ASSERT_NE(board1, board3);
}
