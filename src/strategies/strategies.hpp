//
// Created by Obi Davis on 28/08/2024.
//

#ifndef ELIMINATION_RESULT_HPP
#define ELIMINATION_RESULT_HPP

#include "cell_combination.hpp"
#include "value_combination.hpp"
#include "basic.hpp"
#include "elimination.hpp"

using strategy_fn = std::vector<elimination_result> (*)(const board&);

#include <variant>

#endif //ELIMINATION_RESULT_HPP
