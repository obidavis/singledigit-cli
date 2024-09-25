//
// Created by Obi Davis on 28/08/2024.
//

#include "board/cell.hpp"
#include "gtest/gtest.h"

// Test for default constructor
TEST(CellTest, DefaultConstructor) {
    cell c(A1);
    EXPECT_FALSE(c.is_solved());
    EXPECT_DEBUG_DEATH((void)c.value(), "");
    value_set vs({1, 2, 3, 4, 5, 6, 7, 8, 9});
    EXPECT_EQ(c.candidates(), vs);
}

// Test for constructor with a value
TEST(CellTest, ConstructorWithValue) {
    cell c(A1, 5);
    EXPECT_TRUE(c.is_solved());
    EXPECT_EQ(c.value(), 5);
    EXPECT_DEBUG_DEATH((void)c.candidates(), "");
}

// Test for constructor with value_set as candidates
TEST(CellTest, ConstructorWithCandidates) {
    value_set vs({1, 2, 3});
    cell c(A1, vs);
    EXPECT_FALSE(c.is_solved());
    EXPECT_DEBUG_DEATH((void)c.value(), "");
    EXPECT_EQ(c.candidates(), vs);

    value_set vs2({7});
    cell c2(A1, vs2);
    EXPECT_FALSE(c2.is_solved());
    EXPECT_DEBUG_DEATH((void)c2.value(), "");
    EXPECT_EQ(c2.candidates(), vs2);
}

// Test for solve method
TEST(CellTest, SolveMethod) {
    cell c(A1);
    c.solve(4);
    EXPECT_TRUE(c.is_solved());
    EXPECT_EQ(c.value(), 4);

    c.solve(7);
    EXPECT_TRUE(c.is_solved());
    EXPECT_EQ(c.value(), 7);
}

// Test for is_solved method
TEST(CellTest, IsSolved) {
    cell c(A1);
    EXPECT_FALSE(c.is_solved());

    c.solve(3);
    EXPECT_TRUE(c.is_solved());
}

// Test for candidates method
TEST(CellTest, Candidates) {
    value_set vs({2, 4, 6});
    cell c(A1, vs);
    EXPECT_EQ(c.candidates().to_string(), "{2, 4, 6}");

    c.solve(3);
    EXPECT_DEBUG_DEATH((void)c.candidates(), "");
}

// Test for value method
TEST(CellTest, Value) {
    cell c(A1);
    EXPECT_DEBUG_DEATH((void)c.value(), "");

    c.solve(5);
    EXPECT_EQ(c.value(), 5);

    value_set vs({1, 2, 3});
    cell c2(A1, vs);
    EXPECT_DEBUG_DEATH((void)c2.value(), "");

    c2.solve(7);
    EXPECT_EQ(c2.value(), 7);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

