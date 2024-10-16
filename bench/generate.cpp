//
// Created by Obi Davis on 16/10/2024.
//

#include "benchmark/benchmark.h"
#include "generator.hpp"
#include "grader.hpp"

static void BM_generate(benchmark::State &state) {
    for (auto _ : state) {
        puzzle_generator generator(0);
        int g = 0;
        do {
            std::string solution = generator.generate_solution();
            std::string puzzle = generator.generate_puzzle(solution);
            std::vector solve_path = solve(board{puzzle}, all_strategies);
            g = grade(solve_path).grade();
        } while (g < 150);
    }
}

BENCHMARK(BM_generate);

BENCHMARK_MAIN();

