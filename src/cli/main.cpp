//
// Created by Obi Davis on 13/09/2024.
//

#include "cxxopts.hpp"
#include "generate.hpp"
#include "solve.hpp"
#include <fmt/core.h>
#include <chrono>
#include <iostream>

std::pair<int, int> parse_difficulty(std::string_view diff) {
    if (diff == "kids") {
        return {0, 25};
    } else if (diff == "easy") {
        return {25, 75};
    } else if (diff == "moderate") {
        return {75, 125};
    } else if (diff == "tough") {
        return {125, 200};
    } else if (diff == "hard") {
        return {200, std::numeric_limits<int>::max()};
    } else if (diff == "any"){
        return {std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
    } else {
        throw std::invalid_argument("Invalid difficulty: " + std::string(diff));
    }
}

int main(int argc, char **argv) {
    cxxopts::Options options("sudokugen", "Sudoku puzzle generator and solver");
    options.add_options()
        ("h, help", "Print help")
        ("v, verb", "CLI Action (solve, generate)", cxxopts::value<std::string>())
        ("p, puzzle", "Puzzle to solve", cxxopts::value<std::string>())
        ("o, output", "Output format (plain, json)", cxxopts::value<std::string>()->default_value("plain"))
        ("f, full-solution", "Output full solution", cxxopts::value<bool>()->default_value("false"))
        ("s, seed", "Random seed (omit for random device)", cxxopts::value<unsigned int>())
        ("c, count", "Number of puzzles to generate_puzzle", cxxopts::value<int>()->default_value("1"))
        ("d, difficulty", "Difficulty of generated puzzles", cxxopts::value<std::string>()->default_value("any"))
        ("S, solution", "Solution to generate_puzzle puzzles for", cxxopts::value<std::string>())
        ("V, verbose", "Print additional information", cxxopts::value<bool>()->default_value("false"))
        ("j, threads", "Concurrency", cxxopts::value<int>()->default_value("1"));

    try {
        options.parse_positional({"verb", "puzzle"});

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            fmt::println("{}", options.help());
            return 0;
        }

        if (!result.count("verb")) {
            std::cerr << "No action specified\n";
            return 1;
        }

        std::string verb = result["verb"].as<std::string>();

        if (verb == "generate") {
            if (result.count("puzzle")) {
                std::cerr << "Puzzle should not be specified for generate\n";
                return 1;
            }
            unsigned seed = result.count("seed")
                ? result["seed"].as<unsigned>()
                : std::random_device{}();

            auto [min_difficulty, max_difficulty] = parse_difficulty(result["difficulty"].as<std::string>());
            auto now = std::chrono::high_resolution_clock::now();
            std::vector generated = generate({
                .min_difficulty = min_difficulty,
                .max_difficulty = max_difficulty,
                .seed = seed,
                .num_puzzles = result["count"].as<int>(),
                .threads = result["threads"].as<int>()
            });
            auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
            for (const auto &puzzle : generated) {
                fmt::println("{}", puzzle.puzzle.to_short_string());
                fmt::println("\t Difficulty: {}", puzzle.breakdown.grade());
            }
            fmt::println("Generated {} puzzles in {}ms", generated.size(), elapsed.count());
        } else if (verb == "solve") {
            if (!result.count("puzzle")) {
                std::cerr << "Puzzle must be specified for solve\n";
                return 1;
            }
            std::string puzzle = result["puzzle"].as<std::string>();
            if (!validate_puzzle_str(puzzle)) {
                std::cerr << "Invalid puzzle: " << puzzle << "\n";
                return 1;
            }
            auto solution = solve(puzzle);
            for (const auto &step : solution) {
                fmt::println("{}", step.to_string());
            }
            return 0;
        } else {
            std::cerr << "Unknown action: " << verb << "\n";
            return 1;
        }
    } catch (const cxxopts::exceptions::exception &e) {
        std::cerr << "Error parsing options: " << e.what() << "\n";
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
