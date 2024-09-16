//
// Created by Obi Davis on 13/09/2024.
//

#include "cxxopts.hpp"
#include "generator.hpp"
#include "grader.hpp"
#include <print>
#include <chrono>

int main(int argc, char **argv) {
    cxxopts::Options options("sudokugen", "Sudoku puzzle generator and solver");
    options.add_options()
        ("c, count", "Number of puzzles to generate", cxxopts::value<int>()->default_value("1"))
        ("s,seed", "Random seed", cxxopts::value<int>()->default_value("0"))
        ("n,solution", "Solution to generate puzzle from", cxxopts::value<std::string>())
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Print usage");
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::println("{}", options.help());
        return 0;
    }
    int seed = result["seed"].as<int>();
    int count = result["count"].as<int>();
    bool verbose = result["verbose"].as<bool>();
    std::string solution = generate_complete_board(seed);
    if (result.count("solution")) {
        solution = result["solution"].as<std::string>();
        // TODO: validate solution
    }
    if (verbose) {
        std::println("Generating {} puzzles for solution {} with seed {}",
            count, solution, seed);
    }
    puzzle_generator generator(seed);
    for (int i = 0; i < count; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        auto puzzle = generator.generate(solution);
        auto end = std::chrono::high_resolution_clock::now();
        std::println("Puzzle {}: {}", i, puzzle);
        if (verbose) {
            std::println("Generated in {}ms",
                duration_cast<std::chrono::milliseconds>(end - start).count());
            auto solution_steps = solve(board{puzzle}, all_strategies);
            auto breakdown = grade(solution_steps);
            std::println("Elimination points: {}", breakdown.elimination_points);
            std::println("Solution points: {}", breakdown.solution_points);
            std::println("Average solutions per step: {}", breakdown.average_solutions_per_step);
            std::println("Grade: {}", breakdown.grade());
        }
    }
    return 0;
}
