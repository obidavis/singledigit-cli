//
// Created by Obi Davis on 16/10/2024.
//

#include "format.hpp"

#include "fmt/core.h"
#include "nlohmann/json.hpp"

#include <iostream>

#include "solver.hpp"
#include "generate.hpp"

using nlohmann::json;

void to_json(json &j, const cell_solution &solution) {
    j = {
        {"cell", index_to_string(solution.cell)},
        {"value", solution.value}
    };
}

void to_json(json &j, const cell_set &cs) {
    j = json::array();
    for (int i = 0; i < 81; ++i) {
        if (cs.at(i)) {
            j.push_back(index_to_string(cell_index{i}));
        }
    }
}

void to_json(json &j, const value_set &vs) {
    j = json::array();
    for (int i = 1; i < 10; ++i) {
        if (vs.at(i)) {
            j.push_back(i);
        }
    }
}

void to_json(json &j, const base_elimination &elim) {
    j = {
        {"eliminatedCells", elim.eliminated_cells},
        {"eliminatedValues", elim.eliminated_values},
        {"comment", elim.to_string()}
    };
}

void to_json(json &j, const std::unique_ptr<base_elimination> &elim) {
    return to_json(j, *elim);
}

void to_json(json &j, const solution_step &step) {
    j = {
        {"eliminationCount", step.total_eliminations},
        {"solutionCount", step.total_solutions},
        {"position", step.starting_position.to_long_string()},
        {"eliminations", step.eliminations},
        {"solutions", step.solutions},
        {"strategy", step.strategy_name}
    };
}

void to_json(json &j, const generation_result &result) {
    j = {
        {"clues", result.clues},
        {"solution", result.solution},
        {"difficulty", std::round(result.difficulty)}
    };
}

void format(const std::vector<generation_result> &results, const format_options &options) {
    if (options.format_type == format_options::type::plain) {
        for (int i = 0; i < results.size(); ++i) {
            fmt::println("------ Sudoku {} ------", i + 1);
            fmt::println("Clues:       {}", results[i].clues);
            fmt::println("Solution:    {}", results[i].solution);
            fmt::println("Difficulty:  {:.0f}", results[i].difficulty);
            std::cout << std::endl;
        }
    } else if (options.format_type == format_options::type::json) {
        json j{{"puzzles", results}};
        std::cout << j.dump(options.indent) << std::endl;
    }
}

void format(const std::vector<solution_step> &steps, const format_options &options) {
    if (options.format_type == format_options::type::plain) {
        for (int step_no = 0; step_no < steps.size(); ++step_no) {
            const solution_step &step = steps[step_no];
            fmt::println("[{:02}] {}: (Candidates Eliminated: {}, Cells Solved: {})", step_no + 1, step.strategy_name, step.total_eliminations, step.total_solutions);
            fmt::println("     Starting Position: {}", step.starting_position.to_long_string());
            for (const auto &elim : step.eliminations) {
                fmt::println("     {} - eliminated {} from {}", elim->to_string(), elim->eliminated_values.to_string(), elim->eliminated_cells.to_string());
            }
            for (const auto &sol : step.solutions) {
                fmt::println("     {} set to {}", index_to_string(sol.cell), sol.value);
            }
            fmt::println("     Ending Position: {}", step.ending_position.to_long_string());
        }
    } else if (options.format_type == format_options::type::json) {
        json j = {
            {"solvePath", steps},
            {"solution", steps.back().ending_position.to_short_string()},
        };
        std::cout << j.dump(options.indent) << std::endl;
    }
}

