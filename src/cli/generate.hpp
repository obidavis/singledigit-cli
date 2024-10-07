//
// Created by Obi Davis on 16/09/2024.
//

#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "generator.hpp"
#include "grader.hpp"

#include <future>
#include <mutex>

struct generation_options {
    float min_difficulty;
    float max_difficulty;
    unsigned int seed;
    int num_puzzles;
    int threads;
};

struct generation_result {
    std::string clues;
    std::string solution;
    std::vector<solution_step> solve_path;
    float difficulty;
};


class thread_safe_seed_generator {
public:
    explicit thread_safe_seed_generator(unsigned int seed) : gen(seed) {}
    unsigned int operator()() {
        std::lock_guard lock(mutex);
        std::uniform_int_distribution<unsigned int> dist;
        return dist(gen);
    }
private:
    std::mt19937 gen;
    std::mutex mutex;
};

inline auto generate(const generation_options &options) {
    auto generate_task = [min = options.min_difficulty, max = options.max_difficulty](thread_safe_seed_generator &gen, unsigned int count) {
        std::vector<generation_result> results;
        while (results.size() < count) {
            puzzle_generator generator(gen());
            std::string solution = generator.generate_solution();
            std::string puzzle = generator.generate_puzzle(solution);
            std::vector solve_path = solve(board{puzzle}, all_strategies);
            auto breakdown = grade(solve_path);
            float difficulty = breakdown.grade();
            if (difficulty >= min && difficulty <= max) {
                results.emplace_back(
                    std::move(puzzle),
                    std::move(solution),
                    std::move(solve_path),
                    difficulty
                    );
            }
        }
        return results;
    };

    thread_safe_seed_generator gen(options.seed);

    std::vector<std::future<std::vector<generation_result>>> futures;
    unsigned int puzzles_per_thread = options.num_puzzles / options.threads;
    futures.reserve(options.threads);
    for (int i = 0; i < options.threads; ++i) {
        futures.push_back(std::async(std::launch::async, generate_task, std::ref(gen), puzzles_per_thread));
    }
    if (options.num_puzzles % options.threads != 0) {
        puzzles_per_thread = options.num_puzzles % options.threads;
        futures.push_back(std::async(std::launch::async, generate_task, std::ref(gen), puzzles_per_thread));
    }
    std::vector<generation_result> results;
    for (auto &f : futures) {
        auto res = f.get();
        results.insert(
            results.end(),
            std::make_move_iterator(res.begin()),
            std::make_move_iterator(res.end())
        );
    }
    return results;
}



#endif //GENERATE_HPP
