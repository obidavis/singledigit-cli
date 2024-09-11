//
// Created by Obi Davis on 09/09/2024.
//

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "../board/board.hpp"
#include "../strategies/strategies.hpp"

struct cell_solution {
    cell_index cell;
    int value;
};

struct solution_step {
    std::vector<cell_solution> solutions;
    std::vector<elimination> eliminations;
    board state;
};

std::vector<solution_step> solve(const board &bd, const std::vector<strategy_fn> &strategies);

#endif //SOLVER_HPP
