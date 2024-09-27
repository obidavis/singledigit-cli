//
// Created by Obi Davis on 09/09/2024.
//

#include "basic.hpp"
#include "../board/cell.hpp"
#include "fmt/core.h"

std::vector<std::unique_ptr<base_elimination>> basic(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> results;
    cell_set solved_cells = bd.solved_cells();
    cell_set open_cells = bd.open_cells();
    for (const cell &c : bd[solved_cells]) {
        const auto [row, col, box] = csets_at_index(c.index());
        cell_set affected_cells = open_cells & (row | col | box);
        int value = c.value();
        cell_set eliminated_cells = bd[affected_cells].where([value](const cell &affected_cell) {
            return affected_cell.candidates().at(value);
        });
        if (!eliminated_cells.empty()) {
            value_set eliminated_values{value};
            cell_index set_cell = c.index();
            basic_elimination result = {
                eliminated_cells,
                eliminated_values,
                set_cell
            };
            results.emplace_back(std::make_unique<basic_elimination>(result));
        }
    }
    return results;
}

std::string basic_elimination::to_string() const {
    return fmt::format("{} in {}",
        eliminated_values.first(),
        index_to_string(set_cell));
}