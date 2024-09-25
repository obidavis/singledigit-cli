//
// Created by Obi Davis on 29/08/2024.
//

#ifndef CELL_COMBINATION_HPP
#define CELL_COMBINATION_HPP

#include <memory>
#include <vector>

#include "base_elimination.hpp"
#include "../board/board.hpp"

template <size_t N>
struct cell_combination_elimination : base_elimination {
    cell_combination_elimination(cell_set eliminated_cells, value_set eliminated_values, cell_set cells, constraint_set c_set)
        : base_elimination(eliminated_cells, eliminated_values), cells(cells), c_set(c_set) {}
    cell_set cells;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const override;
};

std::vector<std::unique_ptr<base_elimination>> naked_singles(const board &bd);
std::vector<std::unique_ptr<base_elimination>> naked_pairs(const board &bd);
std::vector<std::unique_ptr<base_elimination>> naked_triples(const board &bd);
std::vector<std::unique_ptr<base_elimination>> naked_quads(const board &bd);

using naked_single_elimination = cell_combination_elimination<1>;
using naked_pair_elimination = cell_combination_elimination<2>;
using naked_triple_elimination = cell_combination_elimination<3>;
using naked_quad_elimination = cell_combination_elimination<4>;

extern template struct cell_combination_elimination<1>;
extern template struct cell_combination_elimination<2>;
extern template struct cell_combination_elimination<3>;
extern template struct cell_combination_elimination<4>;

#endif //CELL_COMBINATION_HPP
