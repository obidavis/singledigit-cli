//
// Created by Obi Davis on 29/08/2024.
//

#ifndef CELL_COMBINATION_HPP
#define CELL_COMBINATION_HPP

#include <vector>

#include "../board/board.hpp"

template <size_t N>
struct cell_combination_elimination {
    cell_set cells;
    value_set values;
    constraint_set c_set;
    cell_set eliminated_cells;
    [[nodiscard]] std::string to_string() const;
    int apply(board &b) const;
    [[nodiscard]] bool operator==(const cell_combination_elimination &other) const = default;
};

using naked_single_elimination = cell_combination_elimination<1>;
using naked_pair_elimination = cell_combination_elimination<2>;
using naked_triple_elimination = cell_combination_elimination<3>;
using naked_quad_elimination = cell_combination_elimination<4>;

std::vector<naked_single_elimination> naked_singles(const board &bd);
std::vector<naked_pair_elimination> naked_pairs(const board &bd);
std::vector<naked_triple_elimination> naked_triples(const board &bd);
std::vector<naked_quad_elimination> naked_quads(const board &bd);

extern template struct cell_combination_elimination<1>;
extern template struct cell_combination_elimination<2>;
extern template struct cell_combination_elimination<3>;
extern template struct cell_combination_elimination<4>;

#endif //CELL_COMBINATION_HPP
