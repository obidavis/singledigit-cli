#ifndef CONSTRAINT_SET_HPP
#define CONSTRAINT_SET_HPP

#include "cell_set.hpp"

enum class constraint_set_type {
    row,
    column,
    box
};

struct constraint_set : cell_set {
    constraint_set(std::span<const cell, 81> cells, constraint_set_type type, int index);

    [[nodiscard]] std::string to_string() const;

    constraint_set_type type;
    int index;
};

#endif //CONSTRAINT_SET_HPP
