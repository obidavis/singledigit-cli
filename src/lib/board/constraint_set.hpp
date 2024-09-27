#ifndef CONSTRAINT_SET_HPP
#define CONSTRAINT_SET_HPP

#include "cell_set.hpp"

enum class constraint_set_type {
    row,
    column,
    box
};

struct constraint_set : cell_set {
    constraint_set(constraint_set_type type, int index);

    [[nodiscard]] std::string to_string() const;

    constraint_set_type type;
    int index;
};

inline std::array<constraint_set, 3> csets_at_index(cell_index i) {
    int row = i / 9;
    int col = i % 9;
    int box = (row / 3) * 3 + (col / 3);
    return {
        constraint_set(constraint_set_type::row, row),
        constraint_set(constraint_set_type::column, col),
        constraint_set(constraint_set_type::box, box)
    };
}

#endif //CONSTRAINT_SET_HPP
