//
// Created by Obi Davis on 28/08/2024.
//

#ifndef ELIMINATION_RESULT_HPP
#define ELIMINATION_RESULT_HPP

#include "elimination.hpp"
#include "basic.hpp"
#include "cell_combination.hpp"
#include "value_combination.hpp"
#include "intersection_removal.hpp"

using strategy_fn = std::vector<elimination> (*)(const board&);

static const std::vector all_strategies = {
        basic,
        naked_singles,
        hidden_singles,
        naked_pairs,
        naked_triples,
        hidden_pairs,
        hidden_triples,
        hidden_quads,
        naked_quads,
        // pointing_pairs_triples,
        // box_line_reduction
};

#endif //ELIMINATION_RESULT_HPP
