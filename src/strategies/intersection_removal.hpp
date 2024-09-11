//
// Created by Obi Davis on 09/09/2024.
//

#ifndef INTERSECTION_REMOVAL_HPP
#define INTERSECTION_REMOVAL_HPP

#include "elimination.hpp"
#include "../board/cell.hpp"
#include "../board/cell_set.hpp"
#include "../board/constraint_set.hpp"
#include "../board/board.hpp"
#include <vector>
#include <utility>

// struct intersection_removal_result {
//     cell_set intersection;
//     cell_set eliminated_cells;
//     value_set eliminated_values;
//     std::pair<constraint_set, constraint_set> c_sets;
//
//     [[nodiscard]] bool operator==(const intersection_removal_result &) const = default;
//     [[nodiscard]] bool operator!=(const intersection_removal_result &) const = default;
//
//     [[nodiscard]] std::string why() const;
//     // [[nodiscard]] elimination to_elimination() const;
// };

std::vector<elimination> pointing_pairs_triples(const board &bd);
std::vector<elimination> box_line_reduction(const board &bd);

#endif //INTERSECTION_REMOVAL_HPP
