//
// Created by Obi Davis on 30/09/2024.
//

#ifndef FISH_HPP
#define FISH_HPP

#include "base_elimination.hpp"

template <size_t N>
struct fish_elimination : base_elimination {
    fish_elimination(value_set eliminated_values, cell_set eliminated_cells, const std::array<cell_set, N> &sets)
        : base_elimination(eliminated_cells, eliminated_values), sets(sets) {}

    [[nodiscard]] std::string to_string() const override;
    [[nodiscard]] std::string name() const override;
    std::array<cell_set, N> sets;
};

std::vector<std::unique_ptr<base_elimination>> x_wing(const board &bd);
std::vector<std::unique_ptr<base_elimination>> swordfish(const board &bd);



#endif //FISH_HPP
