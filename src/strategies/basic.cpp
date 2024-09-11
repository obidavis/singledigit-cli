//
// Created by Obi Davis on 09/09/2024.
//

#include "basic.hpp"
#include "../board/cell.hpp"
#include <format>

// std::string basic_result::to_string() const {
//     return std::format("{} in {} eliminated from {}", eliminated_value, (int)set_cell, eliminated_cells.to_string());
// }

std::vector<elimination> basic(const board &bd) {
    std::vector<elimination> results;
    for (constraint_set c_set : bd.c_sets()) {
        for (const auto &[index, set_cell] : c_set.closed_cells().indexed_values()) {
            int value = set_cell.value();
            cell_set eliminated_cells = c_set.open_cells().where([value](const cell &open_cell) {
                return open_cell.candidates().at(value);
            });
            if (!eliminated_cells.empty()) {
                basic_elimination result = {
                    .set_cell = cell_index{index},
                    .eliminated_value = value,
                    .eliminated_cells = eliminated_cells,
                    .c_set = c_set
                };
                results.emplace_back(result);
            }
        }
    }
    return results;
}

void basic_elimination::apply(board &b) const {
    for (const auto &[index, cell] : eliminated_cells.indexed_values()) {
        b.eliminate_candidate(cell_index{index}, eliminated_value);
    }
}

std::string basic_elimination::to_string() const {

    return std::format("{} in cell {}, {}, eliminated {} from cells {}",
        eliminated_value,
        index_to_string(set_cell),
        c_set.to_string(),
        eliminated_value,
        eliminated_cells.to_string());
}