//
// Created by Obi Davis on 30/08/2024.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "strategies/basic.hpp"
#include <algorithm>

TEST(BasicStrategy, Test1) {
    // A 17 clue puzzle
    board bd("vuvuvuvu0h03vuvuvuvu21vuvuvuvu05vuvuvuvuvuvuvuvuvuvuvu0905vu21vuvuvuvuvuvuvuvuvu11vuvu0h0341vuvuvuvuvuvuvuvuvuvuvu05vuvu09vuvuvu0h81vuvuvuvuvuvu11vu03vuvuvuvuvuvu");
    std::vector elims = basic(bd);
    std::vector<basic_elimination> basic_elims;
    std::transform(elims.begin(), elims.end(), std::back_inserter(basic_elims), [](const elimination &elim) {
        return std::get<basic_elimination>(elim);
    });

    auto a5_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == A5 && result.eliminated_value == 4;
    });
    EXPECT_EQ(a5_count, 3);

    auto a6_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == A6 && result.eliminated_value == 1;
    });
    EXPECT_EQ(a6_count, 3);

    auto b2_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == B2 && result.eliminated_value == 6;
    });
    EXPECT_EQ(b2_count, 3);

    auto b7_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == B7 && result.eliminated_value == 2;
    });
    EXPECT_EQ(b7_count, 3);

    auto d1_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == D1 && result.eliminated_value == 3;
    });
    EXPECT_EQ(d1_count, 3);

    auto d2_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == D2 && result.eliminated_value == 2;
    });
    EXPECT_EQ(d2_count, 3);

    auto d4_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == D4 && result.eliminated_value == 6;
    });
    EXPECT_EQ(d4_count, 3);

    auto e5_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == E5 && result.eliminated_value == 5;
    });
    EXPECT_EQ(e5_count, 3);

    auto e8_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == E8 && result.eliminated_value == 4;
    });
    EXPECT_EQ(e8_count, 3);

    auto e9_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == E9 && result.eliminated_value == 1;
    });
    EXPECT_EQ(e9_count, 3);

    auto f1_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == F1 && result.eliminated_value == 7;
    });
    EXPECT_EQ(f1_count, 3);

    auto g4_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == G4 && result.eliminated_value == 2;
    });
    EXPECT_EQ(g4_count, 3);

    auto g7_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == G7 && result.eliminated_value == 3;
    });
    EXPECT_EQ(g7_count, 3);

    auto h2_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == H2 && result.eliminated_value == 4;
    });
    EXPECT_EQ(h2_count, 3);

    auto h3_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == H3 && result.eliminated_value == 8;
    });
    EXPECT_EQ(h3_count, 3);

    auto i1_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == I1 && result.eliminated_value == 5;
    });
    EXPECT_EQ(i1_count, 3);

    auto i3_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.set_cell == I3 && result.eliminated_value == 1;
    });
    EXPECT_EQ(i3_count, 3);

    // Should be 17 eliminations per constraint set
    auto row_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.c_set.type == constraint_set_type::row;
    });
    EXPECT_EQ(row_count, 17);

    auto col_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.c_set.type == constraint_set_type::column;
    });
    EXPECT_EQ(col_count, 17);

    auto box_count = std::ranges::count_if(basic_elims, [](const basic_elimination &result) {
        return result.c_set.type == constraint_set_type::box;
    });
    EXPECT_EQ(box_count, 17);

    // Should be 3 * 17 = 51 eliminations in total
    EXPECT_THAT(basic_elims, testing::SizeIs(3 * 17));

    // All open cells in the constraint set should be eliminated
    bool all_open_cells_eliminated = std::ranges::all_of(basic_elims, [&](const basic_elimination &result) {
        return result.eliminated_cells == bd[result.c_set].open_cells();
    });
    EXPECT_TRUE(all_open_cells_eliminated);
}
