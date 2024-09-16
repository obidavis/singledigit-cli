//
// Created by Obi Davis on 30/08/2024.
//

#ifndef BASIC_HPP
#define BASIC_HPP

#include "../board/value_set.hpp"
#include "../board/cell_set.hpp"
#include "../board/board.hpp"
#include <vector>

#include "elimination.hpp"

struct basic_elimination {
    cell_index set_cell;
    int eliminated_value;
    cell_set eliminated_cells;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const;
    int apply(board &b) const;
    [[nodiscard]] bool operator==(const basic_elimination &other) const = default;
};

std::vector<basic_elimination> basic(const board &bd);

#endif //BASIC_HPP
