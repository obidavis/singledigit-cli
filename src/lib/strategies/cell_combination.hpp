//
// Created by Obi Davis on 29/08/2024.
//

#ifndef CELL_COMBINATION_HPP
#define CELL_COMBINATION_HPP

#include <vector>

#include "../board/board.hpp"
#include "elimination.hpp"


// struct cell_combination_result {
//     cell_combination_result(const cell_set &cells, value_set values, const constraint_set &c_set,
//                             const cell_set &eliminated_cells)
//             : cells(cells), values(values), c_set(c_set), eliminated_cells(eliminated_cells) {}
//     cell_set cells;
//     value_set values;
//     constraint_set c_set;
//     cell_set eliminated_cells;
//
//     [[nodiscard]] bool operator==(const cell_combination_result &other) const = default;
//     [[nodiscard]] std::string why() const;
// };

std::vector<elimination> naked_singles(const board &bd);
std::vector<elimination> naked_pairs(const board &bd);
std::vector<elimination> naked_triples(const board &bd);
std::vector<elimination> naked_quads(const board &bd);

#endif //CELL_COMBINATION_HPP
