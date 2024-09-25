//
// Created by Obi Davis on 09/09/2024.
//

#include "solver.hpp"
#include "strategies/strategies.hpp"
#include <algorithm>
#include <sstream>

std::string solution_step::to_string() const {
    std::ostringstream oss;
    std::visit([&oss](const auto &elims) {
        for (const auto &elim : elims) {
            oss << elim.to_string() << '\n';
        }
    }, eliminations);
    for (const auto &sol : solutions) {
        oss << "Solved " << sol.cell << " with " << sol.value << '\n';
    }
    return oss.str();
}

solution_step solve_step(const board &bd, const std::vector<strategy_fn> &strategies) {
    solution_step result = {
        .eliminations = {},
        .total_eliminations = 0,
        .solutions = {},
        .total_solutions = 0,
        .state = bd
    };

    for (strategy_fn strategy : strategies) {
        auto eliminations = strategy(bd);
        if (!empty(eliminations)) {
            result.eliminations = std::move(eliminations);
            std::visit([&]<typename T>(const std::vector<T> &elims) {
                for (const T &elim : elims) {
                    result.total_eliminations += elim.apply(result.state);
                }
            }, result.eliminations);
            // for (const elimination &elimination : result.eliminations) {
            //     result.total_eliminations += std::visit([&result](const auto &elim) {
            //         return elim.apply(result.state);
            //     }, elimination);
            // }
            break;
        }
    }

    for (auto i = cell_index{0}; i < 81; i = cell_index{i + 1}) {
        if (!bd[i].is_solved() && bd[i].candidates().count() == 1) {
            result.solutions.push_back({i, bd[i].candidates().first()});
            result.state[i].solve(bd[i].candidates().first());
            ++result.total_solutions;
        }
    }

    return result;
}

std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies) {
    std::vector<solution_step> steps;
    steps.emplace_back(solve_step(bd, strategies));
    while (!steps.back().state.is_solved()) {
        const auto &step = steps.back();
        if (!step.made_progress() || !step.state.is_valid()) {
            return {};
        }
        steps.emplace_back(solve_step(step.state, strategies));
    }
    return steps;
}
