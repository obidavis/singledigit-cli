//
// Created by Obi Davis on 28/08/2024.
//

#ifndef ELIMINATION_RESULT_HPP
#define ELIMINATION_RESULT_HPP

#include <variant>
#include <functional>

#include "basic.hpp"
#include "cell_combination.hpp"
#include "value_combination.hpp"
#include "intersection_removal.hpp"

using strategy_result = std::variant<
        std::vector<basic_elimination>,
        std::vector<naked_single_elimination>,
        std::vector<naked_pair_elimination>,
        std::vector<naked_triple_elimination>,
        std::vector<naked_quad_elimination>,
        std::vector<hidden_single_elimination>,
        std::vector<hidden_pair_elimination>,
        std::vector<hidden_triple_elimination>,
        std::vector<hidden_quad_elimination>,
        std::vector<intersection_removal_elimination>
>;

using strategy_fn = strategy_result (*)(const board&);

inline bool empty(const strategy_result &result) {
    return std::visit([](const auto &v) {
        return v.empty();
    }, result);
}

template <auto Fun>
auto wrap_strategy() {
    return [](const board &b) -> strategy_result {
        return std::invoke(Fun, b);
    };
}

static const std::vector<strategy_fn> all_strategies = {
    wrap_strategy<basic>(),
    wrap_strategy<naked_singles>(),
    wrap_strategy<hidden_singles>(),
    wrap_strategy<naked_pairs>(),
    wrap_strategy<naked_triples>(),
    wrap_strategy<hidden_pairs>(),
    wrap_strategy<hidden_triples>(),
    wrap_strategy<naked_quads>(),
    wrap_strategy<hidden_quads>(),
    wrap_strategy<pointing_pairs_triples>(),
    wrap_strategy<box_line_reduction>(),
};

#endif //ELIMINATION_RESULT_HPP
