//
// Created by Obi Davis on 26/09/2024.
//

#include "json.hpp"
#include <nlohmann/json.hpp>

void to_json(nlohmann::json &j, const generation_result &result) {
    j = {
        {"clues", result.clues},
        {"solution", result.solution},
        {"difficulty", std::round(result.difficulty)}
    };
}
