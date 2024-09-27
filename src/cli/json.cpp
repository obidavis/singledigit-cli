//
// Created by Obi Davis on 26/09/2024.
//

#include "json.hpp"
#include <nlohmann/json.hpp>

void to_json(nlohmann::json &j, const cell_solution &solution) {
    j = {
        {"cell", index_to_string(solution.cell)},
        {"value", solution.value}
    };
}

void to_json(nlohmann::json &j, const cell_set &cs) {
    j = nlohmann::json::array();
    for (int i = 0; i < 81; ++i) {
        if (cs.at(i)) {
            j.push_back(index_to_string(cell_index{i}));
        }
    }
}

void to_json(nlohmann::json &j, const value_set &vs) {
    j = nlohmann::json::array();
    for (int i = 1; i < 10; ++i) {
        if (vs.at(i)) {
            j.push_back(i);
        }
    }
}

void to_json(nlohmann::json &j, const base_elimination &elim) {
    j = {
        {"eliminatedCells", elim.eliminated_cells},
        {"eliminatedValues", elim.eliminated_values},
        {"comment", elim.to_string()}
    };
}

void to_json(nlohmann::json &j, const std::unique_ptr<base_elimination> &elim) {
    return to_json(j, *elim);
}

void to_json(nlohmann::json &j, const solution_step &step) {
    j = {
        {"eliminationCount", step.total_eliminations},
        {"solutionCount", step.total_solutions},
        {"position", step.starting_position.to_long_string()},
        {"eliminations", step.eliminations},
        {"solutions", step.solutions},
        {"strategy", step.strategy_name}
    };
}

void to_json(nlohmann::json &j, const generation_result &result) {
    j = {
        {"clues", result.clues},
        {"solution", result.solution},
        {"difficulty", std::round(result.difficulty)}
    };
}
