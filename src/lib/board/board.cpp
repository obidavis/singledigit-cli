#include <bit>
#include <charconv>
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

std::array<constraint_set, 27> board::_c_sets = make_c_sets();

board::board(std::string_view board_string) {
    if (board_string.length() != 81 && board_string.length() != 162) {
        throw std::invalid_argument("Board string must be 81 or 162 characters long");
    }
    _cells.reserve(81);
    if (board_string.size() == 81) {
        for (int i = 0; i < 81; i++) {
            _cells.emplace_back(cell_index{i});
            if (const int value = board_string[i] - '0'; value > 0 && value < 10) {
                _cells[i].solve(value);
                _clue_flags.set(i, true);
            }
        }
    } else {
        for (int i = 0; i < 81; ++i) {
            // Extract two characters for each cell
            std::string cell_str = std::string(board_string.substr(i * 2, 2));

            // Convert from base 32 to decimal
            uint32_t n = std::stoul(cell_str, nullptr, 32);

            _clue_flags.set(i, n & 1);
            // set last bit to zero to clear clue flag
            n &= ~1;

            // If the bit count is 1, it's a single candidate (solved cell)
            if (__builtin_popcount(n) == 1) {
                int value = std::bit_width(n) - 1;
                _cells.emplace_back(cell_index{i}, value);
            } else {
                value_set candidates = value_set::from_uint(n);
                _cells.emplace_back(cell_index{i}, candidates);
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

std::string board::to_long_string() const {
    /*
     *  return board.cells.map(cell => {
     *      let n = cell.value ? (1 << cell.value - 1) : 0;
     *      for (let i = 0; i < 9; i++) {
     *          n |= cell.candidates[i] ? 1 << i : 0;
     *      }
     *      n = (n << 1) + (cell.isClue ? 1 : 0);
     *      let h = n.toString(32);
     *      if (h.length < 2) {
     *          h = '0' + h;
     *      }
     *      return h;
     *  }).join('');
     */
    std::string result;
    result.reserve(162);
    for (int i = 0; i < 81; i++) {
        cell c = _cells[i];
        uint32_t n = 0;
        if (c.is_solved()) {
            n = 1 << c.value();
        } else {
            for (int i = 1; i < 10; i++) {
                n |= c.candidates().at(i) ? 1 << i : 0;
            }
        }
        n |= _clue_flags[i] ? 1 : 0;
        char buf[2];
        auto [ptr, ec] = std::to_chars(buf, buf + 2, n, 32);
        if (ptr == buf + 1) {
            buf[1] = buf[0];
            buf[0] = '0';
        }
        result.append(buf, 2);
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

cell_set board::solved_cells() const {
    return [this]() {
        cell_set result;
        for (int i = 0; i < 81; i++) {
            if (_cells[i].is_solved()) {
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

