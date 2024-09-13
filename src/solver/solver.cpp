//
// Created by Obi Davis on 09/09/2024.
//

#include "solver.hpp"
#include "../strategies/basic.hpp"
#include <algorithm>

std::optional<solution_step> solve_step(const board &bd, const std::vector<strategy_fn> &strategies) {
    std::optional result = solution_step {
        .solutions = {},
        .eliminations = {},
        .state = bd
    };

    for (auto [index, cell] : result->state.open_cells().indexed_values()) {
        if (cell.candidates().count() == 1) {
            result->solutions.push_back({index, cell.candidates().first()});
            cell.solve(cell.candidates().first());
        }
    }

    if (!result->solutions.empty()) {
        return result;
    }

    for (strategy_fn strategy : strategies) {
        result->eliminations = std::move(strategy(bd));
        if (!result->eliminations.empty()) {
            for (const elimination &elimination : result->eliminations) {
                elimination.apply(result->state);
            }
            return result;
        }
    }

    return std::nullopt;
}

std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies) {
    std::vector<solution_step> steps;
    const board *current = &bd;
    do {
        auto step = solve_step(*current, strategies);

        if (!step || !step->state.is_valid()) {
            return {};
        }

        current = &step->state;
        steps.emplace_back(std::move(step.value()));

    } while (!current->is_solved());
    return steps;
}
