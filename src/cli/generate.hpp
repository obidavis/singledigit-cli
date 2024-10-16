//
// Created by Obi Davis on 16/09/2024.
//

#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "format.hpp"
#include "generator.hpp"
#include "solver.hpp"
#include "CLI/App.hpp"

struct generation_options {
    std::vector<float> difficulty_range;
    std::vector<strategy_type> allowed_strategies;
    std::vector<strategy_type> required_strategies;
    unsigned int threads;
    unsigned int count;
    unsigned int seed;
    format_options format;
};

struct generation_result {
    std::string clues;
    std::string solution;
    float difficulty;
    bool operator==(const generation_result &rhs) const {
        return clues == rhs.clues;
    }
};

void setup_generate(CLI::App &app);
std::vector<generation_result> run_generate(const generation_options &opt);


#endif //GENERATE_HPP
