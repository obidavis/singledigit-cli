#include "constraint_set.hpp"
#include "fmt/core.h"

static constexpr std::bitset<81> row_mask(int row) {
    std::bitset<81> mask;
    for (int i = 0; i < 9; i++) {
        mask.set(row * 9 + i);
    }
    return mask;
}

static constexpr std::bitset<81> col_mask(int col) {
    std::bitset<81> mask;
    for (int i = 0; i < 9; i++) {
        mask.set(i * 9 + col);
    }
    return mask;
}

static constexpr std::bitset<81> box_mask(int box) {
    std::bitset<81> mask;
    for (int i = 0; i < 9; i++) {
        mask.set((box / 3) * 27 + (box % 3) * 3 + (i / 3) * 9 + (i % 3));
    }
    return mask;
}

static constexpr std::bitset<81> constraint_mask(constraint_set_type type, int index) {
    switch (type) {
        case constraint_set_type::row:
            return row_mask(index);
        case constraint_set_type::column:
            return col_mask(index);
        case constraint_set_type::box:
            return box_mask(index);
        default:
            return {};
    }
}

constraint_set::constraint_set(constraint_set_type type, int index)
    : cell_set(constraint_mask(type, index)), type(type), index(index) {}


std::string constraint_set::to_string() const {
    switch (type) {
        case constraint_set_type::row:
            return fmt::format("Row {}", 'A' + index);
        case constraint_set_type::column:
            return fmt::format("Column {}", index + 1);
        case constraint_set_type::box:
            return fmt::format("Box {}", index + 1);
        default:
            return "Unknown";
    }
}
