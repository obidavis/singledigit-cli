//
// Created by Obi Davis on 09/09/2024.
//

#ifndef INTERSECTION_REMOVAL_HPP
#define INTERSECTION_REMOVAL_HPP

#include "elimination.hpp"
#include "../board/board.hpp"
#include <vector>

struct intersection_removal_elimination {
    cell_set intersection;
    cell_set eliminated_cells;
    value_set eliminated_values;
    std::pair<constraint_set, constraint_set> c_sets;
    [[nodiscard]] std::string to_string() const;
    int apply(board &b) const;
    [[nodiscard]] bool operator==(const intersection_removal_elimination &other) const = default;
};

std::vector<intersection_removal_elimination> pointing_pairs_triples(const board &bd);
std::vector<intersection_removal_elimination> box_line_reduction(const board &bd);

#endif //INTERSECTION_REMOVAL_HPP
