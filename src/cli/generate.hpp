//
// Created by Obi Davis on 16/09/2024.
//

#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "generator.hpp"
#include "grader.hpp"

#include <future>
#include <fmt/core.h>

struct generation_options {
    int min_difficulty;
    int max_difficulty;
    unsigned int seed;
    int num_puzzles;
    int threads;
};

struct generation_result {
    board puzzle;
    std::vector<solution_step> solve_path;
    grade_breakdown breakdown;
};

inline auto generate(const generation_options &options) {
    auto generate_task = [min = options.min_difficulty, max = options.max_difficulty](unsigned int seed, unsigned int count) {
        std::vector<generation_result> results;
        puzzle_generator generator(seed);
        while (results.size() < count) {
            std::string solution = generator.generate_solution();
            std::string puzzle = generator.generate_puzzle(solution);
            std::vector solve_path = solve(board{puzzle}, all_strategies);
            auto breakdown = grade(solve_path);
            float grade = breakdown.grade();
            if (grade >= min && grade <= max) {
                results.push_back({board{puzzle}, std::move(solve_path), breakdown});
            }
        }
        return results;
    };
    std::vector<std::future<std::vector<generation_result>>> futures;
    unsigned int puzzles_per_thread = options.num_puzzles / options.threads;
    futures.reserve(options.threads);
    for (int i = 0; i < options.threads; ++i) {
        futures.push_back(std::async(std::launch::async, generate_task, options.seed + i, puzzles_per_thread));
    }
    if (options.num_puzzles % options.threads != 0) {
        puzzles_per_thread = options.num_puzzles % options.threads;
        futures.push_back(std::async(std::launch::async, generate_task, options.seed + options.threads, puzzles_per_thread));
    }
    std::vector<generation_result> results;
    auto now = std::chrono::high_resolution_clock::now();
    for (auto &f : futures) {
        auto res = f.get();
        results.insert(
            results.end(),
            std::make_move_iterator(res.begin()),
            std::make_move_iterator(res.end())
        );
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = duration_cast<std::chrono::milliseconds>(end - now);
    fmt::println("{} ms to join", elapsed.count());
    return results;
}


#endif //GENERATE_HPP
