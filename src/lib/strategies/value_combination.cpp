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
                    eliminated_values,
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
    return fmt::format("{} with {} found in {}, cells {}",
                       name(),
                       values.to_string(),
                       c_set.to_string(),
                       eliminated_cells.to_string());


}

template<size_t N>
std::string value_combination_elimination<N>::name() const {
    if constexpr (N == 1) {
        return "Hidden Single";
    } else if constexpr (N == 2) {
        return "Hidden Pair";
    } else if constexpr (N == 3) {
        return "Hidden Triple";
    } else if constexpr (N == 4) {
        return "Hidden Quad";
    } else {
        return "Hidden N-Tuple";
    }
}

template<size_t N>
strategy_type value_combination_elimination<N>::type() const {
    if constexpr (N == 1) {
        return strategy_type::hidden_singles;
    } else if constexpr (N == 2) {
        return strategy_type::hidden_pairs;
    } else if constexpr (N == 3) {
        return strategy_type::hidden_triples;
    } else if constexpr (N == 4) {
        return strategy_type::hidden_quads;
    }
}

template struct value_combination_elimination<1>;
template struct value_combination_elimination<2>;
template struct value_combination_elimination<3>;
template struct value_combination_elimination<4>;