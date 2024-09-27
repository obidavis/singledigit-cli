//
// Created by Obi Davis on 25/09/2024.
//

#ifndef BASE_ELIMINATION_HPP
#define BASE_ELIMINATION_HPP

#include "../board/board.hpp"
#include "../board/cell_set.hpp"
#include "../board/value_set.hpp"

struct base_elimination {
    base_elimination(cell_set eliminated_cells, value_set eliminated_values)
        : eliminated_cells(eliminated_cells), eliminated_values(eliminated_values) {}
    cell_set eliminated_cells;
    value_set eliminated_values;
    [[nodiscard]] int apply(board &bd) const {
        int total_eliminations = 0;
        for (cell &c : bd[eliminated_cells]) {
            total_eliminations += c.remove_candidates(eliminated_values);
        }
        return total_eliminations;
    }
    [[nodiscard]] virtual std::string to_string() const = 0;
    [[nodiscard]] virtual std::string name() const = 0;
    virtual ~base_elimination() = default;
};

#endif //BASE_ELIMINATION_HPP
