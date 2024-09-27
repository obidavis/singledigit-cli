//
// Created by Obi Davis on 13/09/2024.
//

#include "cell_set.hpp"

#include <sstream>

std::string cell_set::to_string() const {
    std::ostringstream oss;
    oss << '[';
    bool first = true;
    for (int i = 0; i < 81; ++i) {
        if (mask.test(i)) {
            if (!first) {
                oss << ", ";
            }
            oss << index_to_string(cell_index{i});
            first = false;
        }
    }
    oss << ']';
    return oss.str();
}
