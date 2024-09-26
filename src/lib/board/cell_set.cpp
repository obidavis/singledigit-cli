//
// Created by Obi Davis on 13/09/2024.
//

#include "cell_set.hpp"

#include <sstream>

std::string cell_set::to_string() const {
    std::ostringstream oss;
    oss << '[';
    for (int i = 0; i < 81; ++i) {
        if (mask.test(i)) {
            oss << index_to_string(cell_index{i}) << ", ";
        }
    }
    if (mask.any()) {
        oss.seekp(-2, std::ios_base::end);
    }
    oss << ']';
    return oss.str();
}
