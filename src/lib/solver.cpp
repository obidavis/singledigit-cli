//
// Created by Obi Davis on 09/09/2024.
//

#include "solver.hpp"
#include "strategies/strategies.hpp"
#include <algorithm>
#include <sstream>

std::string solution_step::to_string() const {
    std::ostringstream oss;
    for (const auto &elim : eliminations) {
        oss << elim->to_string() << '\n';
    }
    for (const auto &sol : solutions) {
        oss << "Solved " << index_to_string(sol.cell) << " with " << sol.value << '\n';
    }
    return oss.str();
}

solution_step solve_step(const board &bd, const std::vector<strategy_fn> &strategies) {
    solution_step result = {
        .eliminations = {},
        .total_eliminations = 0,
        .solutions = {},
        .total_solutions = 0,
        .starting_position = bd,
        .ending_position = bd
    };

    for (strategy_fn strategy : strategies) {
        auto eliminations = strategy(bd);
        if (!empty(eliminations)) {
            result.eliminations = std::move(eliminations);
            for (const auto &elim : result.eliminations) {
                result.total_eliminations += elim->apply(result.ending_position);
            }
            break;
        }
    }

    for (auto i = cell_index{0}; i < 81; i = cell_index{i + 1}) {
        if (!bd[i].is_solved() && bd[i].candidates().count() == 1) {
            result.solutions.push_back({i, bd[i].candidates().first()});
            result.ending_position[i].solve(bd[i].candidates().first());
            ++result.total_solutions;
        }
    }

    return result;
}

std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies) {
    std::vector<solution_step> steps;
    steps.emplace_back(solve_step(bd, strategies));
    while (!steps.back().ending_position.is_solved()) {
        const auto &step = steps.back();
        if (!step.made_progress() || !step.ending_position.is_valid()) {
            return {};
        }
        steps.emplace_back(solve_step(step.ending_position, strategies));
    }
    return steps;
}
