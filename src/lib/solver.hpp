//
// Created by Obi Davis on 09/09/2024.
//

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "board/board.hpp"
#include "strategies/strategies.hpp"

struct cell_solution {
    cell_index cell;
    int value;
};

struct solution_step {
    strategy_result eliminations;
    int total_eliminations;
    std::vector<cell_solution> solutions;
    int total_solutions;
    board state;
    [[nodiscard]] bool made_progress() const {
        return !empty(eliminations) || !solutions.empty();
    }
    [[nodiscard]] std::string to_string() const;
};

solution_step solve_step(const board &bd, const std::vector<strategy_fn> &strategies);
std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies);


#endif //SOLVER_HPP
