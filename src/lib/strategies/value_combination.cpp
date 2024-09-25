//
// Created by Obi Davis on 28/08/2024.
//
#include "board/cell_set.hpp"
#include "board/value_set.hpp"
#include "value_combination.hpp"
#include <fmt/core.h>

template <size_t N>
static std::vector<std::unique_ptr<base_elimination>> value_combination(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> eliminations;

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
                    cells_containing_values,
                    ~vs,
                    vs,
                    c_set
                };
                eliminations.emplace_back(std::make_unique<value_combination_elimination<N>>(elim));
            }
        }
    }
    return eliminations;
}


std::vector<std::unique_ptr<base_elimination>> hidden_singles(const board &bd) {
    return value_combination<1>(bd);
}

std::vector<std::unique_ptr<base_elimination>> hidden_pairs(const board &bd) {
    return value_combination<2>(bd);
}

std::vector<std::unique_ptr<base_elimination>> hidden_triples(const board &bd) {
    return value_combination<3>(bd);
}

std::vector<std::unique_ptr<base_elimination>> hidden_quads(const board &bd) {
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
    return fmt::format("{} in {}, eliminated_cells {} with values {}",
                       reasons[n - 1],
                       c_set.to_string(),
                       eliminated_cells.to_string(),
                       values.to_string());
}

template struct value_combination_elimination<1>;
template struct value_combination_elimination<2>;
template struct value_combination_elimination<3>;
template struct value_combination_elimination<4>;