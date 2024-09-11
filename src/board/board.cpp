#include <bit>
#include <cassert>
#include <ranges>

#include "board.hpp"

board::board() {
    _cells = std::array<cell, 81>{};
}

board::board(std::string_view board_string) {
    assert(board_string.length() == 81 || board_string.length() == 162);
    if (board_string.size() == 81) {
        for (int i = 0; i < 81; i++) {
            if (const int value = board_string[i] - '0'; value > 0 && value < 10) {
                _cells[i].solve(value);
            }
        }
    } else {
        for (int i = 0; i < 81; ++i) {
            // Extract two characters for each cell
            std::string cell_str = std::string(board_string.substr(i * 2, 2));

            // Convert from base 32 to decimal
            uint32_t n = std::stoul(cell_str, nullptr, 32);

            // set last bit to zero to clear clue flag
            n &= ~1;

            // If the bit count is 1, it's a single candidate (solved cell)
            if (__builtin_popcount(n) == 1) {
                int value = std::bit_width(n) - 1;
                _cells[i].solve(value);
            } else {
                value_set candidates = value_set::from_uint(n);
                _cells[i] = cell(candidates);
            }
        }
    }
}

std::string board::to_short_string() const {
    std::string result;
    result.reserve(81);
    for (const cell &c : _cells) {
        result.push_back(c.is_solved() * c.value() + '0');
    }
    return result;
}

bool board::is_valid() const {
    for (const auto &c_set : c_sets()) {
        value_set values{};
        for (const cell &c : c_set) {
            if (c.is_solved()) {
                if (values.at(c.value())) {
                    return false;
                }
                values.set(c.value());
            }
        }
    }
    return true;
}

bool board::is_solved() const {
    return cells().open_cells().empty() && is_valid();
}

cell_set board::cells() const {
    return cell_set{_cells, std::bitset<81>().set()};
}

mutable_cell_set board::cells() {
    return mutable_cell_set{_cells, std::bitset<81>().set()};
}

cell_set board::open_cells() const {
    return cells().open_cells();
}

mutable_cell_set board::open_cells() {
    return cells().open_cells();
}

mutable_cell_set board::closed_cells() {
    return cells().closed_cells();
}


mutable_cell_set board::solved_cells() {
    return cells().closed_cells();
}

cell_set board::cells_at(std::initializer_list<cell_index> indices) const {
    std::bitset<81> mask;
    for (int index : indices) {
        mask.set(index);
    }
    return {_cells, mask};
}

mutable_cell_set board::cells_at(std::initializer_list<cell_index> indices) {
    std::bitset<81> mask;
    for (int index : indices) {
        mask.set(index);
    }
    return {_cells, mask};
}

constraint_set board::row(int index) const {
    return constraint_set(_cells, constraint_set_type::row, index - 1);
}

constraint_set board::col(int index) const {
    return constraint_set(_cells, constraint_set_type::column, index - 1);
}

constraint_set board::box(int index) const {
    return constraint_set(_cells, constraint_set_type::box, index - 1);
}

std::array<constraint_set, 9> board::rows() const {
    return [this]<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(_cells, constraint_set_type::row, Is)...};
    }(std::make_index_sequence<9>{});
}

std::array<constraint_set, 9> board::cols() const {
    return [this]<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(_cells, constraint_set_type::column, Is)...};
    }(std::make_index_sequence<9>{});
}

std::array<constraint_set, 9> board::boxes() const {
    return [this]<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(_cells, constraint_set_type::box, Is)...};
    }(std::make_index_sequence<9>{});
}

std::array<constraint_set, 27> board::c_sets() const {
    auto to_array = [](auto &&... xs) {
        return std::array{xs...};
    };
    return std::apply(to_array, std::tuple_cat(rows(), cols(), boxes()));
}

std::span<cell, 81> board::mutable_cells() {
    return _cells;
}

void board::solve(cell_index index, int value) {
    _cells[index].solve(value);
}

void board::reset(cell_index index) {
    _cells[index] = cell();
}

void board::eliminate_candidate(cell_index index, int value) {
    _cells[index].remove_candidate(value);
}

void board::eliminate_candidates(cell_index index, value_set values) {
    _cells[index].remove_candidates(values);
}
