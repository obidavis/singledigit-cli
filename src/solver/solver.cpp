//
// Created by Obi Davis on 09/09/2024.
//

#include "solver.hpp"
#include "../strategies/basic.hpp"
#include <algorithm>

static solution_step solve_step(board &bd, const std::vector<strategy_fn> &strategies) {
    solution_step step;
    for (auto [index, cell] : bd.open_cells().indexed_values()) {
        if (cell.candidates().count() == 1) {
            step.solutions.push_back({index, cell.candidates().first()});
            cell.solve(cell.candidates().first());
        }
    }

    if (!step.solutions.empty()) {
        step.eliminations = {};
        step.state = bd;
        return step;
    }

    for (strategy_fn strategy : strategies) {
        std::vector<elimination> eliminations = strategy(bd);
        if (!eliminations.empty()) {
            for (const elimination &elimination : eliminations) {
                elimination.apply(bd);
            }
            step.eliminations = eliminations;
            step.state = bd;
            return step;
        }
    }

    return step;
}

std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies) {
    std::vector<solution_step> steps;
    board copy = bd;
    while (!copy.is_solved()) {
        solution_step step = solve_step(copy, strategies);
        steps.push_back(step);
        copy = step.state;
        if (!copy.is_valid()) {
            return {};
        }
    }
    return steps;
}
