//
// Created by Obi Davis on 30/08/2024.
//

#ifndef BASIC_HPP
#define BASIC_HPP

#include "board/value_set.hpp"
#include "board/cell_set.hpp"
#include "board/board.hpp"
#include <vector>

#include "elimination.hpp"

std::vector<elimination> basic(const board &bd);
std::vector<elimination> guess(const board &bd);

#endif //BASIC_HPP
