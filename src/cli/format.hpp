//
// Created by Obi Davis on 16/10/2024.
//

#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <vector>

struct solution_step;
struct generation_result;

struct format_options {
    enum class type {
        plain,
        json
    } format_type;
    int indent;
};

void format(const std::vector<solution_step> &steps, const format_options &options);
void format(const std::vector<generation_result> &results, const format_options &options);


#endif //FORMAT_HPP
