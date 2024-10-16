#ifndef BOARD_HPP
#define BOARD_HPP

#include <string_view>
#include <span>
#include <string>
#include <vector>
#include <bitset>

#include "cell.hpp"
#include "cell_set.hpp"
#include "cells_view.hpp"
#include "constraint_set.hpp"


class board {
public:
    explicit board(std::string_view board_string);

    [[nodiscard]] std::string to_short_string() const;
    [[nodiscard]] std::string to_long_string() const;
    [[nodiscard]] std::string to_full_board_string() const;

    [[nodiscard]] bool is_valid() const;
    [[nodiscard]] bool is_solved() const;

    auto operator[](const cell_set &mask) {
        return cells_view{ _cells.data(), mask };
    }

    auto operator[](const cell_set &mask) const {
        return cells_view{ _cells.data(), mask };
    }

    cell &operator[](cell_index index) {
        return _cells[index];
    }

    const cell &operator[](cell_index index) const {
        return _cells[index];
    }

    [[nodiscard]] cell_set open_cells() const;
    [[nodiscard]] cell_set solved_cells() const;

    [[nodiscard]] constraint_set row(int index) const;
    [[nodiscard]] constraint_set col(int index) const;
    [[nodiscard]] constraint_set box(int index) const;

    [[nodiscard]] std::span<const constraint_set, 9> rows() const {
        return std::span<const constraint_set, 9>{_c_sets.data(), 9};
    }
    [[nodiscard]] std::span<const constraint_set, 9> cols() const {
        return std::span<const constraint_set, 9>{_c_sets.data() + 9, 9};
    }
    [[nodiscard]] std::span<const constraint_set, 9> boxes() const {
        return std::span<const constraint_set, 9>{_c_sets.data() + 18, 9};
    }
    [[nodiscard]] std::span<const constraint_set, 27> c_sets() const {
        return _c_sets;
    }

private:
    std::vector<cell> _cells;
    std::bitset<81> _clue_flags;
    static std::array<constraint_set, 27> _c_sets;
};

#endif //BOARD_HPP
