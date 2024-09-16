//
// Created by Obi Davis on 13/09/2024.
//

#include "grader.hpp"
#include <unordered_map>
#include <typeindex>

static const std::unordered_map<std::type_index, float> scale_factors = {
    {typeid(basic_elimination), 0.1f},
    {typeid(naked_single_elimination), 0.1f},
    {typeid(hidden_single_elimination), 2.0f},
    {typeid(naked_pair_elimination), 2.0f},
    {typeid(naked_triple_elimination), 2.0f},
    {typeid(hidden_pair_elimination), 2.0f},
    {typeid(hidden_triple_elimination), 2.0f},
    {typeid(naked_quad_elimination), 5.0f},
    {typeid(hidden_quad_elimination), 5.0f},
    {typeid(intersection_removal_elimination), 5.0f}
};

grade_breakdown grade(const std::vector<solution_step> &solution) {
    float elimination_points = 0;
    float solutition_points = 0;
    float total_solutions = 0;
    for (const solution_step &step : solution) {
        const float scale_factor = std::visit([]<typename T>(const std::vector<T> &) {
            return scale_factors.at(typeid(T));
        }, step.eliminations);
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
