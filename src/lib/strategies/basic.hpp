//
// Created by Obi Davis on 30/08/2024.
//

#ifndef BASIC_HPP
#define BASIC_HPP

#include "../board/value_set.hpp"
#include "../board/cell_set.hpp"
#include "../board/board.hpp"
#include "base_elimination.hpp"
#include <memory>
#include <vector>

struct basic_elimination : base_elimination {
    basic_elimination(cell_set eliminated_cells, value_set eliminated_values, cell_index set_cell, constraint_set c_set)
        : base_elimination(eliminated_cells, eliminated_values), set_cell(set_cell), c_set(c_set) {}
    cell_index set_cell;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const override;
};

std::vector<std::unique_ptr<base_elimination>> basic(const board &bd);

#endif //BASIC_HPP
