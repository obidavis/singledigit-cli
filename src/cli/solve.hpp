//
// Created by Obi Davis on 16/10/2024.
//

#ifndef SOLVE_HPP
#define SOLVE_HPP

#include "CLI/App.hpp"
#include "format.hpp"
#include <vector>
#include "solver.hpp"

struct solve_options {
    std::string board;
    bool single_step;
    format_options format;
};

void setup_solve(CLI::App &app);
std::vector<solution_step> run_solve(const solve_options &opt);

#endif //SOLVE_HPP
