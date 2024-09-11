#ifndef BOARD_HPP
#define BOARD_HPP

#include <string_view>
#include <string>
#include <array>

#include "cell.hpp"
#include "cell_set.hpp"
#include "constraint_set.hpp"


class board {
public:
    board();
    explicit board(std::string_view board_string);

    [[nodiscard]] std::string to_short_string() const;
    [[nodiscard]] std::string to_long_string() const;
    [[nodiscard]] std::string to_full_board_string() const;

    [[nodiscard]] bool is_valid() const;
    [[nodiscard]] bool is_solved() const;

    [[nodiscard]]
    cell_set cells() const;
    mutable_cell_set cells();

    [[nodiscard]]
    cell_set open_cells() const;
    mutable_cell_set open_cells();

    [[nodiscard]]
    cell_set closed_cells() const;
    mutable_cell_set closed_cells();

    [[nodiscard]]
    cell_set solved_cells() const;
    mutable_cell_set solved_cells();

    [[nodiscard]]
    cell_set cells_at(std::initializer_list<cell_index> indices) const;
    mutable_cell_set cells_at(std::initializer_list<cell_index> indices);

    [[nodiscard]] constraint_set row(int index) const;
    [[nodiscard]] constraint_set col(int index) const;
    [[nodiscard]] constraint_set box(int index) const;

    [[nodiscard]] std::array<constraint_set, 9> rows() const;
    [[nodiscard]] std::array<constraint_set, 9> cols() const;
    [[nodiscard]] std::array<constraint_set, 9> boxes() const;
    [[nodiscard]] std::array<constraint_set, 27> c_sets() const;

    std::span<cell, 81> mutable_cells();
    void solve(cell_index index, int value);
    void reset(cell_index index);
    void eliminate_candidate(cell_index index, int value);
    void eliminate_candidates(cell_index index, value_set values);
private:
    std::array<cell, 81> _cells;
};


#endif //BOARD_HPP
