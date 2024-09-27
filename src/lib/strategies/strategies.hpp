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

using strategy_result = std::vector<std::unique_ptr<base_elimination>>;
using strategy_fn = strategy_result (*)(const board&);

static const std::vector all_strategies = {
    basic,
    hidden_singles,
    naked_pairs,
    naked_triples,
    hidden_pairs,
    hidden_triples,
    naked_quads,
    hidden_quads,
    pointing_pairs_triples,
    box_line_reduction
};

#endif //ELIMINATION_RESULT_HPP
