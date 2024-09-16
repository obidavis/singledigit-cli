//
// Created by Obi Davis on 09/09/2024.
//

#include "intersection_removal.hpp"

#include <format>
#include <sys/fcntl.h>

static void intersection_removal(std::vector<intersection_removal_elimination> &results, const board &bd, const constraint_set &cset1, const constraint_set &cset2) {
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

std::vector<intersection_removal_elimination> pointing_pairs_triples(const board &bd) {
    std::vector<intersection_removal_elimination> results;
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

std::vector<intersection_removal_elimination> box_line_reduction(const board &bd) {
    std::vector<intersection_removal_elimination> results;
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
    return std::format("Intersection Removal: {} in {} eliminated from {}", eliminated_values.to_string(), intersection.to_string(), eliminated_cells.to_string());
}

int intersection_removal_elimination::apply(board &b) const {
    int total_eliminations = 0;
    for (cell &c : b[eliminated_cells]) {
        total_eliminations += c.remove_candidates(eliminated_values);
    }
    return total_eliminations;
}