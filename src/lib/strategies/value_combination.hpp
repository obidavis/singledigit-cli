//
// Created by Obi Davis on 28/08/2024.
//

#ifndef VALUE_COMBINATION_HPP
#define VALUE_COMBINATION_HPP

#include "board/board.hpp"
#include "elimination.hpp"
#include <vector>

// struct value_combination_result {
//     cell_set cells;
//     value_set values;
//     constraint_set constraint_set;
//     [[nodiscard]] std::string why() const;
//     // [[nodiscard]] std::vector<elimination> get_elimination() const;
//
//     [[nodiscard]] bool operator==(const value_combination_result &other) const = default;
// };

std::vector<elimination> hidden_singles(const board &bd);
std::vector<elimination> hidden_pairs(const board &bd);
std::vector<elimination> hidden_triples(const board &bd);
std::vector<elimination> hidden_quads(const board &bd);

#endif //VALUE_COMBINATION_HPP
