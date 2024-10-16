//
// Created by Obi Davis on 09/09/2024.
//

#ifndef INTERSECTION_REMOVAL_HPP
#define INTERSECTION_REMOVAL_HPP

#include "base_elimination.hpp"
#include "../board/board.hpp"
#include <memory>
#include <vector>

struct intersection_removal_elimination : base_elimination {
    intersection_removal_elimination(cell_set eliminated_cells, value_set eliminated_values, cell_set intersection, std::pair<constraint_set, constraint_set> c_sets)
        : base_elimination(eliminated_cells, eliminated_values), intersection(intersection), c_sets(c_sets) {}
    cell_set intersection;
    std::pair<constraint_set, constraint_set> c_sets;
    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string name() const override { return "Intersection Removal"; }
    [[nodiscard]] strategy_type type() const override { return strategy_type::pointing_pairs_triples; }
};

std::vector<std::unique_ptr<base_elimination>> pointing_pairs_triples(const board &bd);
std::vector<std::unique_ptr<base_elimination>> box_line_reduction(const board &bd);

#endif //INTERSECTION_REMOVAL_HPP
