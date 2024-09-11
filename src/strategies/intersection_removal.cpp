//
// Created by Obi Davis on 09/09/2024.
//

#include "intersection_removal.hpp"

static void intersection_removal(std::vector<elimination> &results, const constraint_set &cset1, const constraint_set &cset2) {
    cell_set intersection = cset1.open_cells() & cset2.open_cells();
    if (intersection.empty()) {
        return;
    }
    for (int value = 1; value <= 9; value++) {

        cell_set cells_containing_value = cset1.open_cells().where([value](const cell &cell) {
            return cell.candidates().at(value);
        });

        if (cells_containing_value == intersection) {
            cell_set other_cells = cset2.open_cells() - intersection;
            cell_set eliminated_cells = other_cells.where([value](const cell &cell) {
                return cell.candidates().at(value);
            });
            if (!eliminated_cells.empty()) {
                intersection_removal_elimination result = {
                    .intersection = intersection,
                    .eliminated_cells = eliminated_cells,
                    .eliminated_values = value_set{value},
                    .c_sets = {cset1, cset2}
                };
                results.emplace_back(result);
            }
        }
    }
}

std::vector<elimination> pointing_pairs_triples(const board &bd) {
    std::vector<elimination> results;
    for (constraint_set box : bd.boxes()) {
        for (constraint_set row : bd.rows()) {
            intersection_removal(results, box, row);
        }
        for (constraint_set col : bd.cols()) {
            intersection_removal(results, box, col);
        }
    }
    return results;
}

std::vector<elimination> box_line_reduction(const board &bd) {
    std::vector<elimination> results;
    for (constraint_set box : bd.boxes()) {
        for (constraint_set row : bd.rows()) {
            intersection_removal(results, row, box);
        }
        for (constraint_set col : bd.cols()) {
            intersection_removal(results, col, box);
        }
    }
    return results;
}
