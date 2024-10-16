//
// Created by Obi Davis on 13/09/2024.
//

#include "generate.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(Generate, StableOverMultipleThreads) {
    const generation_options options = {};
    const unsigned int seed = 0;
    const unsigned int count = 10;

    std::vector single_thread_results = generate(options, seed, count, 1);
    std::vector multi_thread_results = generate(options, seed, count, 5);

    EXPECT_THAT(single_thread_results, testing::UnorderedElementsAreArray(multi_thread_results));
}

TEST(Generate, HasRequiredStrats) {
    const generation_options options = {
        .required_strategies = {strategy_type::hidden_singles}
    };
    const unsigned int seed = 0;
    const unsigned int count = 5;
    std::vector results = generate(options, seed, count, 8);
    for (const generation_result &result : results) {
        std::vector solve_path = solve(board{result.clues}, all_strategies);
        bool has_required_strats = std::ranges::any_of(solve_path, [&](const solution_step &step) {
            return std::ranges::any_of(step.eliminations, [&](const std::unique_ptr<base_elimination> &elim) {
                return elim->type() == strategy_type::hidden_singles;
            });
        });
        EXPECT_TRUE(has_required_strats);
    }
}

TEST(Generate, OnlyHasAllowedStrats) {
    const generation_options options = {
        .allowed_strategies = {strategy_type::hidden_singles, strategy_type::hidden_pairs}
    };
}

