//
// Created by Obi Davis on 09/09/2024.
//

#ifndef INTERSECTION_REMOVAL_HPP
#define INTERSECTION_REMOVAL_HPP

#include "elimination.hpp"
#include "../board/board.hpp"
#include <vector>

std::vector<elimination> pointing_pairs_triples(const board &bd);
std::vector<elimination> box_line_reduction(const board &bd);

#endif //INTERSECTION_REMOVAL_HPP
