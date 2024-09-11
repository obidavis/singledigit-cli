//
// Created by Obi Davis on 30/08/2024.
//

#include "cell_combination.hpp"
#include <format>

template <size_t N>
std::vector<elimination> cell_combination(const board &bd) {
    std::vector<elimination> eliminations;

    for (auto c_set : bd.c_sets()) {
        cell_set open_cells = c_set.open_cells();

        for (cell_set cell_combination : combinations(open_cells, N)) {
            value_set values_contained_in_cells = cell_combination.open_values();
            if (values_contained_in_cells.count() == N) {

                cell_set other_cells = open_cells - cell_combination;
                cell_set other_cells_containing_values = other_cells.where([values_contained_in_cells](const cell &cell) {
                    return (cell.candidates() & values_contained_in_cells) != value_set::none();
                });

                if (other_cells_containing_values.empty()) {
                    continue;
                }

                cell_combination_elimination elim = {
                    .cells = cell_combination,
                    .values = values_contained_in_cells,
                    .c_set = c_set,
                    .eliminated_cells = other_cells_containing_values
                };

                eliminations.emplace_back(elim);
            }
        }
    }
    return eliminations;
}

std::string cell_combination_elimination::to_string() const {
    size_t n = cells.count();
    static constexpr std::string_view reasons[] = {
        "Naked Single",
        "Naked Pair",
        "Naked Triple",
        "Naked Quad"
    };
    return std::format("{} in {}, cells {} with values {}",
                       reasons[n - 1],
                       c_set.to_string(),
                       cells.to_string(),
                       values.to_string());
}

std::vector<elimination> naked_singles(const board &bd) {
    return cell_combination<1>(bd);
}

std::vector<elimination> naked_pairs(const board &bd) {
    return cell_combination<2>(bd);
}

std::vector<elimination> naked_triples(const board &bd) {
    return cell_combination<3>(bd);
}

std::vector<elimination> naked_quads(const board &bd) {
    return cell_combination<4>(bd);
}

