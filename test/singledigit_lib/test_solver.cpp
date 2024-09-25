#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "solver.hpp"

TEST(Solver, Solve) {
    testing::internal::CaptureStdout();
    board bd("720096003000205000080004020000000060106503807040000000030800090000702000200430018");
    auto path = solve(bd, all_strategies);
    for (auto step : path) {
        if (!step.solutions.empty()) {
            for (auto solution : step.solutions) {
                std::cout << "Solved cell " << index_to_string(solution.cell) << " with value " << solution.value << "\n";
            }
        }
        if (empty(step.eliminations)) {
            std::visit([]<typename T>(const std::vector<T> &elims) {
                for (const T &elim : elims) {
                    std::cout << elim.to_string() << "\n";
                }
            }, step.eliminations);
        }
    }
    std::string output = testing::internal::GetCapturedStdout();
    SUCCEED() << output;
}

TEST(Solver, MultipleSolutions) {
    board bd("097000563004060009530800040002306080000004050060907100080002035400000700653000410");
    auto path = solve(bd, all_strategies);
    EXPECT_TRUE(path.empty());
}
