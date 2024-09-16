//
// Created by Obi Davis on 30/08/2024.
//

#include "cell_combination.hpp"
#include <format>

template <size_t N>
std::vector<cell_combination_elimination<N>> cell_combination(const board &bd) {
    std::vector<cell_combination_elimination<N>> eliminations;

    for (auto c_set : bd.c_sets()) {
        cell_set open_cells = bd[c_set].open_cells();

        for (cell_set cell_combination : combinations(open_cells, N)) {
            value_set values_contained_in_cells = bd[cell_combination].open_values();
            if (values_contained_in_cells.count() == N) {

                cell_set other_cells = open_cells - cell_combination;
                cell_set other_cells_containing_values = bd[other_cells].where([values_contained_in_cells](const cell &cell) {
                    return (cell.candidates() & values_contained_in_cells) != value_set::none();
                });

                if (other_cells_containing_values.empty()) {
                    continue;
                }

                cell_combination_elimination<N> elim = {
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

std::vector<naked_single_elimination> naked_singles(const board &bd) {
    return cell_combination<1>(bd);
}

std::vector<naked_pair_elimination> naked_pairs(const board &bd) {
    return cell_combination<2>(bd);
}

std::vector<naked_triple_elimination> naked_triples(const board &bd) {
    return cell_combination<3>(bd);
}

std::vector<naked_quad_elimination> naked_quads(const board &bd) {
    return cell_combination<4>(bd);
}

template <size_t N>
std::string cell_combination_elimination<N>::to_string() const {
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

template <size_t N>
int cell_combination_elimination<N>::apply(board &b) const {
    int total_eliminations = 0;
    for (cell &c : b[eliminated_cells]) {
        total_eliminations += c.remove_candidates(values);
    }
    return total_eliminations;
}

template struct cell_combination_elimination<1>;
template struct cell_combination_elimination<2>;
template struct cell_combination_elimination<3>;
template struct cell_combination_elimination<4>;