//
// Created by Obi Davis on 13/09/2024.
//

#include "cxxopts.hpp"
#include "generate.hpp"
#include "output.hpp"
#include "json.hpp"
#include <nlohmann/json.hpp>

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
        return {200, 1000};
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
        ("puzzle", "Puzzle to solve", cxxopts::value<std::string>())
        ("f, format", "Output format (plain, json)", cxxopts::value<std::string>()->default_value("plain"))
        ("i, indent", "Indentation level for JSON output (-1 for unformatted)", cxxopts::value<int>()->default_value("2"))
        ("p, solve-path", "Output solve path for generated puzzles", cxxopts::value<bool>()->default_value("false"))
        ("s, seed", "Random seed (omit for random device)", cxxopts::value<unsigned int>())
        ("c, count", "Number of puzzles to generate", cxxopts::value<int>()->default_value("1"))
        ("d, difficulty", "Difficulty of generated puzzles", cxxopts::value<std::string>()->default_value("any"))
        ("j, threads", "Concurrency", cxxopts::value<int>()->default_value("1"))
        ("S, single-step", "Output just the first step of the solution", cxxopts::value<bool>()->default_value("false"));

    try {
        options.parse_positional({"verb", "puzzle"});

        auto parsed_options = options.parse(argc, argv);

        if (parsed_options.count("help")) {
            fmt::println("{}", options.help());
            return 0;
        }

        if (!parsed_options.count("verb")) {
            std::cerr << "No action specified\n";
            return 1;
        }

        std::string verb = parsed_options["verb"].as<std::string>();

        if (verb == "generate") {
            if (parsed_options.count("puzzle")) {
                std::cerr << "Puzzle should not be specified for generate\n";
                return 1;
            }
            unsigned seed = parsed_options.count("seed")
                ? parsed_options["seed"].as<unsigned>()
                : std::random_device{}();

            auto [min_difficulty, max_difficulty] = parse_difficulty(parsed_options["difficulty"].as<std::string>());
            fmt::print(std::clog, "Generating {} puzzles\n", parsed_options["count"].as<int>());
            fmt::print(std::clog, "Difficulty: {} - {}\n", min_difficulty, max_difficulty);
            fmt::print(std::clog, "Seed: {}\n", seed);
            fmt::print(std::clog, "Threads: {}\n", parsed_options["threads"].as<int>());

            auto now = std::chrono::high_resolution_clock::now();
            std::vector generated_puzzles = generate({
                .min_difficulty = min_difficulty,
                .max_difficulty = max_difficulty,
                .seed = seed,
                .num_puzzles = parsed_options["count"].as<int>(),
                .threads = parsed_options["threads"].as<int>()
            });
            auto elapsed = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
            if (parsed_options["format"].as<std::string>() == "json") {
                nlohmann::json j = {{"puzzles", nlohmann::json::array()}};
                for (const auto & puzzle: generated_puzzles) {
                    j["puzzles"].push_back(puzzle);
                    if (parsed_options["solve-path"].as<bool>()) {
                        j["puzzles"].back()["solve_path"] = puzzle.solve_path;
                    }
                }
                std::cout << j.dump(parsed_options["indent"].as<int>()) << std::endl;
            } else if (parsed_options["format"].as<std::string>() == "plain") {
                for (int i = 0; i < generated_puzzles.size(); ++i) {
                    fmt::println("------ Sudoku {} ------", i + 1);
                    fmt::println("Clues:       {}", generated_puzzles[i].clues);
                    fmt::println("Solution:    {}", generated_puzzles[i].solution);
                    fmt::println("Difficulty:  {:.0f}", generated_puzzles[i].difficulty);
                    if (parsed_options["solve-path"].as<bool>()) {
                        fmt::println("Solve Path:");
                        print_solution_path_plain(generated_puzzles[i].solve_path);
                    }
                    std::cout << std::endl;
                }
            }
            fmt::print(std::clog, "Generated {} puzzles in {} ms\n", generated_puzzles.size(), elapsed.count());
        } else if (verb == "solve") {
            if (!parsed_options.count("puzzle")) {
                std::cerr << "Puzzle must be specified for solve\n";
                return 1;
            }
            std::string puzzle_str = parsed_options["puzzle"].as<std::string>();
            try {
                board bd(puzzle_str);
                if (parsed_options["single-step"].as<bool>()) {
                    solution_step step = solve_step(bd, all_strategies);
                    if (parsed_options["format"].as<std::string>() == "json") {
                        nlohmann::json j = {{"solvePath", {step}}};
                        std::cout << j.dump(parsed_options["indent"].as<int>()) << std::endl;
                    } else if (parsed_options["format"].as<std::string>() == "plain") {
                        print_solution_step_plain(step);
                    }
                } else {
                    std::vector<solution_step> solution_steps = solve(bd, all_strategies);
                    if (parsed_options["format"].as<std::string>() == "json") {
                        nlohmann::json j = {
                            {"solvePath", solution_steps},
                            {"solution", solution_steps.back().ending_position.to_short_string()},
                        };
                        std::cout << j.dump(parsed_options["indent"].as<int>()) << std::endl;
                    } else if (parsed_options["format"].as<std::string>() == "plain") {
                        print_solution_path_plain(solution_steps);
                    }
                }
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
