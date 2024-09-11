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

#endif //ELIMINATION_RESULT_HPP
