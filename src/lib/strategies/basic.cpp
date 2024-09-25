//
// Created by Obi Davis on 09/09/2024.
//

#include "basic.hpp"
#include "../board/cell.hpp"
#include "fmt/core.h"

std::vector<std::unique_ptr<base_elimination>> basic(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> results;
    for (constraint_set c_set : bd.c_sets()) {
        cell_set solved_cells = bd[c_set].solved_cells();
        for (const cell &c : bd[solved_cells]) {
            int value = c.value();
            cell_set open_cells = bd[c_set].open_cells();
            cell_set eliminated_cells = bd[open_cells].where([value](const cell &open_cell) {
                return open_cell.candidates().at(value);
            });
            if (!eliminated_cells.empty()) {
                value_set eliminated_values{value};
                cell_index set_cell = c.index();
                basic_elimination result = {
                    eliminated_cells,
                    eliminated_values,
                    set_cell,
                    c_set
                };
                results.emplace_back(std::make_unique<basic_elimination>(result));
            }
        }
    }
    return results;
}

std::string basic_elimination::to_string() const {
    return fmt::format("{} in cell {}, {}, eliminated {} from cells {}",
        eliminated_values.first(),
        index_to_string(set_cell),
        c_set.to_string(),
        eliminated_values.to_string(),
        eliminated_cells.to_string());
}