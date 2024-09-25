//
// Created by Obi Davis on 13/09/2024.
//

#include "grader.hpp"
#include <unordered_map>
#include <typeindex>
#include <limits>

static float get_scale_factor(const strategy_result &result) {
    if (result.empty()) {
        return 0.0f;
    }
    base_elimination *ptr = result[0].get();
    if (dynamic_cast<basic_elimination *>(ptr)) {
        return 0.1f;
    }
    if (dynamic_cast<naked_single_elimination *>(ptr)) {
        return 0.1f;
    }
    if (dynamic_cast<hidden_single_elimination *>(ptr)) {
        return 2.0f;
    }
    if (dynamic_cast<naked_pair_elimination *>(ptr)) {
        return 2.0f;
    }
    if (dynamic_cast<hidden_pair_elimination *>(ptr)) {
        return 2.0f;
    }
    if (dynamic_cast<naked_triple_elimination *>(ptr)) {
        return 2.0f;
    }
    if (dynamic_cast<hidden_triple_elimination *>(ptr)) {
        return 2.0f;
    }
    if (dynamic_cast<naked_quad_elimination *>(ptr)) {
        return 5.0f;
    }
    if (dynamic_cast<hidden_quad_elimination *>(ptr)) {
        return 5.0f;
    }
    if (dynamic_cast<intersection_removal_elimination *>(ptr)) {
        return 5.0f;
    }
    throw std::invalid_argument("Unknown elimination type");
}
grade_breakdown grade(const std::vector<solution_step> &solution) {
    if (solution.empty()) {
        return {
            .elimination_points = std::numeric_limits<float>::infinity(),
            .solution_points = std::numeric_limits<float>::infinity(),
            .average_solutions_per_step = 0
        };
    }
    float elimination_points = 0;
    float solutition_points = 0;
    float total_solutions = 0;
    for (const solution_step &step : solution) {
        const float scale_factor = get_scale_factor(step.eliminations);
        elimination_points += step.total_eliminations * scale_factor;
        solutition_points += step.total_solutions * scale_factor;
        total_solutions += step.total_solutions;
    }
    return {
        .elimination_points = elimination_points,
        .solution_points = solutition_points,
        .average_solutions_per_step = total_solutions / solution.size()
    };
}
