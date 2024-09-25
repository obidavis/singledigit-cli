//
// Created by Obi Davis on 28/08/2024.
//
#include "board/cell_set.hpp"
#include "board/value_set.hpp"
#include "value_combination.hpp"
#include <format>

template <size_t N>
static std::vector<value_combination_elimination<N>> value_combination(const board &bd) {
    std::vector<value_combination_elimination<N>> eliminations;

    for (auto c_set : bd.c_sets()) {

        value_set open_values = bd[c_set].open_values();
        cell_set open_cells = bd[c_set].open_cells();

        if (open_values.count() < N) {
            continue;
        }

        for (value_set vs : combinations(open_values, N)) {
            cell_set cells_containing_values = bd[open_cells].where([=](const cell &cell) {
                return (cell.candidates() & vs) != value_set::none();
            });
            if (cells_containing_values.count() == N) {
                value_set eliminated_values = bd[cells_containing_values].open_values() - vs;
                if (eliminated_values.empty()) {
                    continue;
                }
                value_combination_elimination<N> elim = {
                    .eliminated_cells = cells_containing_values,
                    .values = vs,
                    .c_set = c_set
                };
                eliminations.emplace_back(elim);
            }
        }
    }
    return eliminations;
}


std::vector<hidden_single_elimination> hidden_singles(const board &bd) {
    return value_combination<1>(bd);
}

std::vector<hidden_pair_elimination> hidden_pairs(const board &bd) {
    return value_combination<2>(bd);
}

std::vector<hidden_triple_elimination> hidden_triples(const board &bd) {
    return value_combination<3>(bd);
}

std::vector<hidden_quad_elimination> hidden_quads(const board &bd) {
    return value_combination<4>(bd);
}

template <size_t N>
std::string value_combination_elimination<N>::to_string() const {
    size_t n = eliminated_cells.count();
    static constexpr std::string_view reasons[] = {
        "Hidden Single",
        "Hidden Pair",
        "Hidden Triple",
        "Hidden Quad"
    };
    return std::format("{} in {}, eliminated_cells {} with values {}",
                       reasons[n - 1],
                       c_set.to_string(),
                       eliminated_cells.to_string(),
                       values.to_string());
}

template <size_t N>
int value_combination_elimination<N>::apply(board &b) const {
    int total_eliminations = 0;
    value_set values_complement = ~values;
    for (cell &cell : b[eliminated_cells]) {
        total_eliminations += cell.remove_candidates(values_complement);
    }
    return total_eliminations;
}

template struct value_combination_elimination<1>;
template struct value_combination_elimination<2>;
template struct value_combination_elimination<3>;
template struct value_combination_elimination<4>;