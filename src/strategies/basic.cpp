//
// Created by Obi Davis on 09/09/2024.
//

#include "basic.hpp"
#include "../board/cell.hpp"
#include <format>

std::vector<elimination> basic(const board &bd) {
    std::vector<elimination> results;
    for (constraint_set c_set : bd.c_sets()) {
        cell_set solved_cells = bd[c_set].solved_cells();
        for (const cell &c : bd[solved_cells]) {
            int value = c.value();
            cell_set open_cells = bd[c_set].open_cells();
            cell_set eliminated_cells = bd[open_cells].where([value](const cell &open_cell) {
                return open_cell.candidates().at(value);
            });
            if (!eliminated_cells.empty()) {
                basic_elimination result = {
                    .set_cell = c.index(),
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
    for (cell &cell : b[eliminated_cells]) {
        cell.remove_candidate(eliminated_value);
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