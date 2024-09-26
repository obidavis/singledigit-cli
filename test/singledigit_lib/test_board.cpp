//
// Created by Obi Davis on 26/09/2024.
//

#include "board/board.hpp"
#include "gtest/gtest.h"

TEST(Board, LongStringConversions) {
    std::string_view board_string = "0922g13486960hf634052i1i418qg1a8b2381i81413k0u1ei8363c41110i8021020509g0210a0ag1060h401081g00581081140200h038i4q0u0kcscc11g12c9ggo1s03os2188844190k82114scdc03840h";
    board bd(board_string);
    std::string long_string = bd.to_long_string();
    EXPECT_EQ(long_string, board_string);
}

