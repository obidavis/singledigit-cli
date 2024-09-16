//
// Created by Obi Davis on 13/09/2024.
//

#ifndef GRADER_HPP
#define GRADER_HPP

#include "solver.hpp"

struct grade_breakdown {
    float elimination_points;
    float solution_points;
    float average_solutions_per_step;
    [[nodiscard]] float grade() const {
        return (elimination_points + solution_points) / average_solutions_per_step;
    }
};

grade_breakdown grade(const std::vector<solution_step> &solution);

#endif //GRADER_HPP
