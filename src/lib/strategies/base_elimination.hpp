//
// Created by Obi Davis on 25/09/2024.
//

#ifndef BASE_ELIMINATION_HPP
#define BASE_ELIMINATION_HPP

#include "../board/board.hpp"
#include "../board/cell_set.hpp"
#include "../board/value_set.hpp"

enum class strategy_type : size_t {
    basic = 0,
    naked_singles,
    hidden_singles,
    naked_pairs,
    naked_triples,
    hidden_pairs,
    hidden_triples,
    naked_quads,
    hidden_quads,
    pointing_pairs_triples,
    box_line_reduction,
    x_wing,
    swordfish,
};

inline const std::vector all_strategy_types = {
    strategy_type::basic,
    strategy_type::naked_singles,
    strategy_type::hidden_singles,
    strategy_type::naked_pairs,
    strategy_type::naked_triples,
    strategy_type::hidden_pairs,
    strategy_type::hidden_triples,
    strategy_type::naked_quads,
    strategy_type::hidden_quads,
    strategy_type::pointing_pairs_triples,
    strategy_type::box_line_reduction,
    strategy_type::x_wing,
    strategy_type::swordfish,
};

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
    [[nodiscard]] virtual strategy_type type() const = 0;
    virtual ~base_elimination() = default;
};

#endif //BASE_ELIMINATION_HPP
