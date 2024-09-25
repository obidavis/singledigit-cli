#include <bit>
#include <cassert>
#include <algorithm>
#include <ranges>

#include "board.hpp"

static std::array<constraint_set, 9> make_rows() {
    return []<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(constraint_set_type::row, Is)...};
    }(std::make_index_sequence<9>{});
}

static std::array<constraint_set, 9> make_cols() {
    return []<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(constraint_set_type::column, Is)...};
    }(std::make_index_sequence<9>{});
}

static std::array<constraint_set, 9> make_boxes() {
    return []<std::size_t ...Is>(std::index_sequence<Is...>) {
        return std::array{constraint_set(constraint_set_type::box, Is)...};
    }(std::make_index_sequence<9>{});
}

static std::array<constraint_set, 27> make_c_sets() {
    auto to_array = [](auto &&... xs) {
        return std::array{xs...};
    };
    return std::apply(to_array, std::tuple_cat(make_rows(), make_cols(), make_boxes()));
}

board::board(std::string_view board_string)
    : _cells([]<size_t... Is>(std::index_sequence<Is...>) {
        return std::array<cell, 81>{cell{cell_index{Is}}...};
        }(std::make_index_sequence<81>{}))
    , _c_sets(make_c_sets()) {

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
                    _cells[i] = cell(cell_index{i}, candidates);
                }
            }
        }
}

std::string board::to_short_string() const {
    std::string result;
    result.reserve(81);
    for (const cell &c : _cells) {
        if (c.is_solved()) {
            result.push_back('0' + c.value());
        } else {
            result.push_back('0');
        }
    }
    return result;
}

bool board::is_valid() const {
    for (const auto &c_set : c_sets()) {
        value_set values{};
        for (const cell &c : (*this)[c_set]) {
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
    return std::ranges::all_of(_cells, [](const cell &c) {
        return c.is_solved();
    }) && is_valid();
}

cell_set board::open_cells() const {
    return [this]() {
        cell_set result;
        for (int i = 0; i < 81; i++) {
            if (!_cells[i].is_solved()) {
                result.set(i, true);
            }
        }
        return result;
    }();
}

constraint_set board::row(int index) const {
    return {constraint_set_type::row, index - 1};
}

constraint_set board::col(int index) const {
    return {constraint_set_type::column, index - 1};
}

constraint_set board::box(int index) const {
    return {constraint_set_type::box, index - 1};
}

