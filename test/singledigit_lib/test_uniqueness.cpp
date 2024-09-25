//
// Created by Obi Davis on 17/09/2024.
//
#include "gtest/gtest.h"
#include "uniqueness.hpp"

TEST(Uniqueness, Unique) {
    EXPECT_TRUE(is_unique("123456789456789123789123456234567891567891234891234567345678912678912345912345678"));
    EXPECT_TRUE(is_unique("309000400200709000087000000750060230600904008028050041000000590000106007006000104"));
    EXPECT_TRUE(is_unique("720096003000205000080004020000000060106503807040000000030800090000702000200430018"));
}

TEST(Uniqueness, NotUnique) {
    EXPECT_FALSE(is_unique("720096003000205000080004020000000060106503807040000000030800090000702000200430010"));
    EXPECT_FALSE(is_unique("000002100701800600030900000050003700000000030002100080000001050009005302008200000"));
}
