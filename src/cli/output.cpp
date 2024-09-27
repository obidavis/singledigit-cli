//
// Created by Obi Davis on 26/09/2024.
//

#include "output.hpp"

void print_solution_step_plain(const solution_step &step) {
    fmt::println("{}: (Candidates Eliminated: {}, Cells Solved: {})", step.strategy_name, step.total_eliminations, step.total_solutions);
    fmt::println("Starting position: {}", step.starting_position.to_long_string());
    for (const auto &elim : step.eliminations) {
        fmt::println("     {} - eliminated {} from {}", elim->to_string(), elim->eliminated_values.to_string(), elim->eliminated_cells.to_string());
    }
    for (const auto &sol : step.solutions) {
        fmt::println("     Cell {} set to {}", index_to_string(sol.cell), sol.value);
    }
    fmt::println("Resulting position: {}", step.ending_position.to_long_string());
}

void print_solution_path_plain(const std::vector<solution_step> &steps) {
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
}
