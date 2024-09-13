//
// Created by Obi Davis on 28/08/2024.
//
#include "board/cell_set.hpp"
#include "board/value_set.hpp"
#include "value_combination.hpp"
#include <format>

template <size_t N>
static std::vector<elimination> value_combination(const board &bd) {
    std::vector<elimination> eliminations;

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
                value_combination_elimination elim = {
                    .cells = cells_containing_values,
                    .values = vs,
                    .c_set = c_set
                };
                eliminations.emplace_back(elim);
            }
        }
    }
    return eliminations;
}


std::vector<elimination> hidden_singles(const board &bd) {
    return value_combination<1>(bd);
}

std::vector<elimination> hidden_pairs(const board &bd) {
    return value_combination<2>(bd);
}

std::vector<elimination> hidden_triples(const board &bd) {
    return value_combination<3>(bd);
}

std::vector<elimination> hidden_quads(const board &bd) {
    return value_combination<4>(bd);
}

std::string value_combination_elimination::to_string() const {
    size_t n = cells.count();
    static constexpr std::string_view reasons[] = {
        "Hidden Single",
        "Hidden Pair",
        "Hidden Triple",
        "Hidden Quad"
    };
    return std::format("{} in {}, cells {} with values {}",
                       reasons[n - 1],
                       c_set.to_string(),
                       cells.to_string(),
                       values.to_string());
}

void value_combination_elimination::apply(board &b) const {
    value_set values_complement = ~values;
    for (cell &cell : b[cells]) {
        cell.remove_candidates(values_complement);
    }
}