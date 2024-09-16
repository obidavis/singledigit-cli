//
// Created by Obi Davis on 28/08/2024.
//

#ifndef VALUE_COMBINATION_HPP
#define VALUE_COMBINATION_HPP

#include "board/board.hpp"
#include "elimination.hpp"
#include <vector>

template <size_t N>
struct value_combination_elimination {
    cell_set cells;
    value_set values;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const;
    int apply(board &b) const;
    [[nodiscard]] bool operator==(const value_combination_elimination &other) const = default;
};

using hidden_single_elimination = value_combination_elimination<1>;
using hidden_pair_elimination = value_combination_elimination<2>;
using hidden_triple_elimination = value_combination_elimination<3>;
using hidden_quad_elimination = value_combination_elimination<4>;

std::vector<hidden_single_elimination> hidden_singles(const board &bd);
std::vector<hidden_pair_elimination> hidden_pairs(const board &bd);
std::vector<hidden_triple_elimination> hidden_triples(const board &bd);
std::vector<hidden_quad_elimination> hidden_quads(const board &bd);

extern template struct value_combination_elimination<1>;
extern template struct value_combination_elimination<2>;
extern template struct value_combination_elimination<3>;
extern template struct value_combination_elimination<4>;

#endif //VALUE_COMBINATION_HPP
