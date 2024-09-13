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
        if (!step.eliminations.empty()) {
            for (auto elimination : step.eliminations) {
                std::cout << elimination.to_string() << "\n";
            }
        }
    }
    std::string output = testing::internal::GetCapturedStdout();
    SUCCEED() << output;
}
