//
// Created by Obi Davis on 10/09/2024.
//

#ifndef COMPLETE_BOARD_HPP
#define COMPLETE_BOARD_HPP

#include <random>
#include <string>

std::string generate_complete_board(int seed);

class puzzle_generator {
public:
    explicit puzzle_generator(unsigned int seed) : gen(seed) {}
    std::string generate_puzzle(std::string_view solution);
    std::string generate_solution();
private:
    std::mt19937 gen;
};

#endif //COMPLETE_BOARD_HPP
