//
// Created by Obi Davis on 09/09/2024.
//

#include "intersection_removal.hpp"

#include <fmt/core.h>

static void intersection_removal(std::vector<std::unique_ptr<base_elimination>> &results, const board &bd, const constraint_set &cset1, const constraint_set &cset2) {
    cell_set intersection = bd[cset1].open_cells() & bd[cset2].open_cells();
    if (intersection.empty()) {
        return;
    }
    for (int value = 1; value <= 9; value++) {
        cell_set open_cells = bd[cset1].open_cells();
        cell_set cells_containing_value = bd[open_cells].where([value](const cell &cell) {
            return cell.candidates().at(value);
        });

        if (cells_containing_value == intersection) {
            cell_set other_cells = bd[cset2].open_cells() - intersection;
            cell_set eliminated_cells = bd[other_cells].where([value](const cell &cell) {
                return cell.candidates().at(value);
            });
            if (!eliminated_cells.empty()) {
                intersection_removal_elimination result = {
                    eliminated_cells,
                    value_set{value},
                    intersection,
                    {cset1, cset2}
                };
                results.emplace_back(std::make_unique<intersection_removal_elimination>(result));
            }
        }
    }
}

std::vector<std::unique_ptr<base_elimination>> pointing_pairs_triples(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> results;
    for (constraint_set box : bd.boxes()) {
        for (constraint_set row : bd.rows()) {
            intersection_removal(results, bd, box, row);
        }
        for (constraint_set col : bd.cols()) {
            intersection_removal(results, bd, box, col);
        }
    }
    return results;
}

std::vector<std::unique_ptr<base_elimination>> box_line_reduction(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> results;
    for (constraint_set box : bd.boxes()) {
        for (constraint_set row : bd.rows()) {
            intersection_removal(results, bd, row, box);
        }
        for (constraint_set col : bd.cols()) {
            intersection_removal(results, bd, col, box);
        }
    }
    return results;
}

std::string intersection_removal_elimination::to_string() const {
    return fmt::format("Intersection Removal: {} in {} eliminated from {}", eliminated_values.to_string(), intersection.to_string(), eliminated_cells.to_string());
}