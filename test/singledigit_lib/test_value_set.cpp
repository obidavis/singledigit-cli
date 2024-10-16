#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include "board/value_set.hpp"  // Replace with the actual header file name

// Test for the value_set constructor with initializer list
TEST(ValueSetTest, ConstructorWithInitializerList) {
    value_set vs({1, 3, 5});
    EXPECT_TRUE(vs.at(1));
    EXPECT_TRUE(vs.at(3));
    EXPECT_TRUE(vs.at(5));
    EXPECT_FALSE(vs.at(2));
}

// Test for the value_set to_string method
TEST(ValueSetTest, ToString) {
    value_set vs({1, 3, 5});
    EXPECT_EQ(vs.to_string(), "1/3/5");  // Adjust based on your implementation

    value_set vs2({1, 2, 3, 4, 5, 6, 7, 8, 9});
    EXPECT_EQ(vs2.to_string(), "1/2/3/4/5/6/7/8/9");

    value_set vs3{};
    EXPECT_EQ(vs3.to_string(), "");
}

// Test for the combinations free function
TEST(CombinationsTest, Functionality) {
    value_set vs({1, 2, 3});
    auto combs = combinations(vs, 2);

    std::vector expected_combinations = {
        value_set({1, 2}),
        value_set({1, 3}),
        value_set({2, 3})
    };

    std::vector<value_set> actual_combinations(combs.begin(), combs.end());

    EXPECT_THAT(actual_combinations, testing::UnorderedElementsAreArray(expected_combinations));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
