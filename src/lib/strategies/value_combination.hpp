//
// Created by Obi Davis on 28/08/2024.
//

#ifndef VALUE_COMBINATION_HPP
#define VALUE_COMBINATION_HPP

#include "base_elimination.hpp"
#include "board/board.hpp"
#include <vector>
#include <memory>

template <size_t N>
struct value_combination_elimination : base_elimination {
    value_combination_elimination(cell_set eliminated_cells, value_set eliminated_values, value_set values, constraint_set c_set)
        : base_elimination(eliminated_cells, eliminated_values), values(values), c_set(c_set) {}
    value_set values;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const override;
};

std::vector<std::unique_ptr<base_elimination>> hidden_singles(const board &bd);
std::vector<std::unique_ptr<base_elimination>> hidden_pairs(const board &bd);
std::vector<std::unique_ptr<base_elimination>> hidden_triples(const board &bd);
std::vector<std::unique_ptr<base_elimination>> hidden_quads(const board &bd);

using hidden_single_elimination = value_combination_elimination<1>;
using hidden_pair_elimination = value_combination_elimination<2>;
using hidden_triple_elimination = value_combination_elimination<3>;
using hidden_quad_elimination = value_combination_elimination<4>;

extern template struct value_combination_elimination<1>;
extern template struct value_combination_elimination<2>;
extern template struct value_combination_elimination<3>;
extern template struct value_combination_elimination<4>;

#endif //VALUE_COMBINATION_HPP
