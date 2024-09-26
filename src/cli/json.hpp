//
// Created by Obi Davis on 26/09/2024.
//

#ifndef JSON_HPP
#define JSON_HPP

#include <nlohmann/json_fwd.hpp>
#include "solver.hpp"
#include "generate.hpp"

void to_json(nlohmann::json &j, const solution_step &step);
void to_json(nlohmann::json &j, const generation_result &result);

#endif //JSON_HPP
