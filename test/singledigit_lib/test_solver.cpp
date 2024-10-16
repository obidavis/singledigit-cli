#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "solver.hpp"

TEST(Solver, MultipleSolutions) {
    board bd("097000563004060009530800040002306080000004050060907100080002035400000700653000410");
    auto path = solve(bd, all_strategies);
    EXPECT_TRUE(path.empty());
}
