//
// Created by Obi Davis on 16/09/2024.
//

#ifndef SOLVE_HPP
#define SOLVE_HPP

#include "grader.hpp"
#include "solver.hpp"

inline auto solve(std::string_view puzzle_str) {
    return solve(board{puzzle_str}, all_strategies);
}

inline bool validate_puzzle_str(std::string_view puzzle_str) {
    bool correct_length = puzzle_str.size() == 81;
    bool valid_characters = std::all_of(puzzle_str.begin(), puzzle_str.end(), [](char c) {
        return c == '.' || (c >= '0' && c <= '9');
    });
    return correct_length && valid_characters;
}

#endif //SOLVE_HPP
