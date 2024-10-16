//
// Created by Obi Davis on 28/08/2024.
//

#ifndef ELIMINATION_RESULT_HPP
#define ELIMINATION_RESULT_HPP

#include <variant>
#include <array>
#include <functional>

#include "basic.hpp"
#include "cell_combination.hpp"
#include "value_combination.hpp"
#include "intersection_removal.hpp"
#include "fish.hpp"

using strategy_result = std::vector<std::unique_ptr<base_elimination>>;
using strategy_fn = strategy_result (*)(const board&);

static constexpr std::array strategy_map = {
    basic,
    naked_singles,
    hidden_singles,
    naked_pairs,
    naked_triples,
    hidden_pairs,
    hidden_triples,
    naked_quads,
    hidden_quads,
    pointing_pairs_triples,
    box_line_reduction,
    x_wing,
    swordfish,
};

static const std::vector all_strategies(strategy_map.begin(), strategy_map.end());

#define STRATEGY_FN(strategy) strategy_map[static_cast<size_t>(strategy)]
#define STRATEGY_MAP_TEST(name) static_assert(strategy_map[static_cast<size_t>(strategy_type::name)] == name);
STRATEGY_MAP_TEST(basic)
STRATEGY_MAP_TEST(naked_singles)
STRATEGY_MAP_TEST(hidden_singles)
STRATEGY_MAP_TEST(naked_pairs)
STRATEGY_MAP_TEST(naked_triples)
STRATEGY_MAP_TEST(hidden_pairs)
STRATEGY_MAP_TEST(hidden_triples)
STRATEGY_MAP_TEST(naked_quads)
STRATEGY_MAP_TEST(hidden_quads)
STRATEGY_MAP_TEST(pointing_pairs_triples)
STRATEGY_MAP_TEST(box_line_reduction)
STRATEGY_MAP_TEST(x_wing)
STRATEGY_MAP_TEST(swordfish)


#endif //ELIMINATION_RESULT_HPP
