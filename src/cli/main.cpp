//
// Created by Obi Davis on 13/09/2024.
//

#include "cxxopts.hpp"
#include "generate.hpp"
#include "output.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <chrono>
#include <iostream>

std::pair<float, float> parse_difficulty(std::string_view diff) {
    if (diff == "kids") {
        return {0, 25};
    } else if (diff == "easy") {
        return {25, 75};
    } else if (diff == "moderate") {
        return {75, 100};
    } else if (diff == "tough") {
        return {100, 200};
    } else if (diff == "hard") {
        return {200, std::numeric_limits<float>::infinity()};
    } else if (diff == "any"){
        return {0, std::numeric_limits<float>::infinity()};
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
            fmt::print(std::clog, "Generating {} puzzles\n", result["count"].as<int>());
            fmt::print(std::clog, "Difficulty: {} - {}\n", min_difficulty, max_difficulty);
            fmt::print(std::clog, "Seed: {}\n", seed);
            fmt::print(std::clog, "Threads: {}\n", result["threads"].as<int>());

            auto now = std::chrono::high_resolution_clock::now();
            std::vector generated = generate({
                .min_difficulty = min_difficulty,
                .max_difficulty = max_difficulty,
                .seed = seed,
                .num_puzzles = result["count"].as<int>(),
                .threads = result["threads"].as<int>()
            });
            auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
            for (int i = 0; i < generated.size(); ++i) {
                fmt::println("------ Sudoku {} ------", i + 1);
                fmt::println("Clues:       {}", generated[i].clues);
                fmt::println("Solution:    {}", generated[i].solution);
                fmt::println("Difficulty:  {:.0f}", generated[i].difficulty);
                if (result["full-solution"].as<bool>()) {
                    fmt::println("Solve Path:");
                    print_solution_path_plain(generated[i].solve_path);
                }
                std::cout << std::endl;
            }
            fmt::print(std::clog, "Generated {} puzzles in {} ms\n", generated.size(), elapsed.count());
        } else if (verb == "solve") {
            if (!result.count("puzzle")) {
                std::cerr << "Puzzle must be specified for solve\n";
                return 1;
            }
            std::string puzzle_str = result["puzzle"].as<std::string>();
            try {
                board bd(puzzle_str);
                std::vector<solution_step> solution_steps = solve(bd, all_strategies);
                print_solution_path_plain(solution_steps);
                return 0;
            } catch (const std::exception &e) {
                std::cerr << "Invalid puzzle: " << e.what() << "\n";
                return 1;
            }
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
