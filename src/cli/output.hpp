//
// Created by Obi Davis on 26/09/2024.
//

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>
#include <vector>

#include <fmt/core.h>

#include "solver.hpp"

void print_solution_step_plain(const solution_step &step);
void print_solution_path_plain(const std::vector<solution_step> &steps);


#endif //OUTPUT_HPP
