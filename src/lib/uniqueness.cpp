//
// Created by Obi Davis on 17/09/2024.
//

#include "uniqueness.hpp"

#include <solver.hpp>

#include "strategies/basic.hpp"
#include "strategies/value_combination.hpp"
#include "strategies/cell_combination.hpp"
#include "board/board.hpp"

template <typename T>
static bool apply_strategy(board &bd, const std::vector<T> &elims) {
    cell_set eliminated_cells{};
    for (const auto &elim : elims) {
        elim.apply(bd);
        eliminated_cells |= elim.eliminated_cells;
    }
    for (cell &c : bd[eliminated_cells]) {
        int count = c.candidates().count();
        if (count == 0) {
            return false;
        }
        if (count == 1) {
            c.solve(c.candidates().first());
        }
    }
    return true;
}

static void is_unique_backtrack(int &solution_count, board bd) { // NOLINT(*-no-recursion)

    auto basic_elims = basic(bd);
    if (!apply_strategy(bd, basic_elims)) {
        return;
    }
    // if (basic_elims.empty()) {
    //
    // }
    //
    if (!bd.is_valid()) {
        return;
    }

    if (bd.is_solved()) {
        ++solution_count;
        return;
    }

    auto open_cells = bd.open_cells();
    auto open_cells_view = bd[open_cells];
    auto next_cell_it = std::ranges::min_element(open_cells_view, [](const cell &a, const cell &b) {
        return a.candidates().count() < b.candidates().count();
    });
    if (next_cell_it == open_cells_view.end()) {
        return;
    }
    cell &next_cell = *next_cell_it;

    value_set candidates = next_cell.candidates();
    for (int value : candidates) {
        next_cell.solve(value);
        is_unique_backtrack(solution_count, bd);
        if (solution_count > 1) {
            return;
        }
        next_cell = cell(next_cell.index(), candidates);
    }
}

class simple_board {
public:
    struct cset_indices { int row, col, box; };
    simple_board(std::string_view board_string) : board_string(board_string), rows{}, cols{}, boxes{} {
        for (int i = 0; i < 81; ++i) {
            if (board_string[i] != '0') {
                int value = board_string[i] - '1';
                auto [row, col, box] = calculate_cset_indices(i);
                rows[row].set(value);
                cols[col].set(value);
                boxes[box].set(value);
            }
        }
    }

    static cset_indices calculate_cset_indices(int index) {
        int row = index / 9;
        int col = index % 9;
        int box = row / 3 * 3 + col / 3;
        return {row, col, box};
    }

    static int calculate_index(cset_indices indices) {
        return indices.row * 9 + indices.col;
    }

    [[nodiscard]]
    std::optional<cset_indices> next_empty_cell(int index) const {
        for (int i = index; i < 81; ++i) {
            if (board_string[i] == '0') {
                return calculate_cset_indices(i);
            }
        }
        return std::nullopt;
    }

    int solve() {
        int solution_count = 0;
        solve_backtracking(0, solution_count);
        return solution_count;
    }

    bool is_unique() {
        int solution_count = 0;
        solve_backtracking<true>(0, solution_count);
        return solution_count <= 1;
    }

    template <bool stop_after_second_solution = false>
    bool solve_backtracking(int start, int &solution_count) {
        auto next_indices = next_empty_cell(start);
        if (!next_indices) {
            return true;
        }

        auto contains = rows[next_indices->row] | cols[next_indices->col] | boxes[next_indices->box];
        if (contains.all()) {
            return false;
        }
        int index = calculate_index(*next_indices);
        for (int i = 0; i < 9; ++i) {
            if (!contains[i]) {
                rows[next_indices->row].set(i);
                cols[next_indices->col].set(i);
                boxes[next_indices->box].set(i);
                board_string[index] = '1' + i;
                if (solve_backtracking<stop_after_second_solution>(index, solution_count)) {
                    ++solution_count;
                    if constexpr (stop_after_second_solution) {
                        return solution_count > 1;
                    }
                }
                rows[next_indices->row].reset(i);
                cols[next_indices->col].reset(i);
                boxes[next_indices->box].reset(i);
            }
        }
        board_string[index] = '0';
        return false;
    }

    static constexpr int N = 9;
    std::string board_string;
    std::bitset<N> rows[N];
    std::bitset<N> cols[N];
    std::bitset<N> boxes[N];
};

bool is_unique(std::string_view board_string) {
    simple_board bd(board_string);
    return bd.is_unique();
}
