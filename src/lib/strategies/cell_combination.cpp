//
// Created by Obi Davis on 30/08/2024.
//

#include "cell_combination.hpp"
#include <fmt/core.h>

template <size_t N>
std::vector<std::unique_ptr<base_elimination>> cell_combination(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> eliminations;

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
                    other_cells_containing_values,
                    values_contained_in_cells,
                    cell_combination,
                    c_set
                };

                eliminations.emplace_back(std::make_unique<cell_combination_elimination<N>>(elim));
            }
        }
    }
    return eliminations;
}

std::vector<std::unique_ptr<base_elimination>> naked_singles(const board &bd) {
    return cell_combination<1>(bd);
}

std::vector<std::unique_ptr<base_elimination>> naked_pairs(const board &bd) {
    return cell_combination<2>(bd);
}

std::vector<std::unique_ptr<base_elimination>> naked_triples(const board &bd) {
    return cell_combination<3>(bd);
}

std::vector<std::unique_ptr<base_elimination>> naked_quads(const board &bd) {
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
    return fmt::format("{} in {}, cells {} with values {}",
                       reasons[n - 1],
                       c_set.to_string(),
                       cells.to_string(),
                       eliminated_values.to_string());
}

template<size_t N>
std::string cell_combination_elimination<N>::name() const {
    if constexpr (N == 1) {
        return "Naked Single";
    } else if constexpr (N == 2) {
        return "Naked Pair";
    } else if constexpr (N == 3) {
        return "Naked Triple";
    } else if constexpr (N == 4) {
        return "Naked Quad";
    } else {
        return "Naked N-Tuple";
    }
}

template <size_t N>
strategy_type cell_combination_elimination<N>::type() const {
    if constexpr (N == 1) {
        return strategy_type::naked_singles;
    } else if constexpr (N == 2) {
        return strategy_type::naked_pairs;
    } else if constexpr (N == 3) {
        return strategy_type::naked_triples;
    } else if constexpr (N == 4) {
        return strategy_type::naked_quads;
    }
}

template struct cell_combination_elimination<1>;
template struct cell_combination_elimination<2>;
template struct cell_combination_elimination<3>;
template struct cell_combination_elimination<4>;