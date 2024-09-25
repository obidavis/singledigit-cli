//
// Created by Obi Davis on 10/09/2024.
//

#include "generator.hpp"

#include <algorithm>
#include <span>
#include <random>
#include <vector>
#include <ranges>

#include "solver.hpp"
#include "uniqueness.hpp"

static constexpr auto default_board_str =
    "123456789"
    "456789123"
    "789123456"
    "231564897"
    "564897231"
    "897231564"
    "312645978"
    "645978312"
    "978312645";

static constexpr int N = 9;

static int index_2d(int row, int col) {
    return row * N + col;
}

static void swap_rows(std::span<char> b, int block_row, std::mt19937 &gen) {
    std::uniform_int_distribution<> dis(0, 2);
    int row1 = block_row * 3 + dis(gen);
    int row2 = block_row * 3 + dis(gen);

    if (row1 != row2) {
        for (int col = 0; col < N; ++col) {
            std::swap(b[index_2d(row1, col)], b[index_2d(row2, col)]);
        }
    }
}

static void swap_columns(std::span<char> b, int block_col, std::mt19937 &gen) {
    std::uniform_int_distribution<> dis(0, 2);
    int col1 = block_col * 3 + dis(gen);
    int col2 = block_col * 3 + dis(gen);

    if (col1 != col2) {
        for (int row = 0; row < N; ++row) {
            std::swap(b[index_2d(row, col1)], b[index_2d(row, col2)]);
        }
    }
}

static void swap_row_blocks(std::span<char> b, std::mt19937 &gen) {
    std::uniform_int_distribution<> dis(0, 2);
    int block1 = dis(gen);
    int block2 = dis(gen);

    if (block1 != block2) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < N; ++col) {
                std::swap(b[index_2d(block1 * 3 + row, col)], b[index_2d(block2 * 3 + row, col)]);
            }
        }
    }
}

static void swap_column_blocks(std::span<char> b, std::mt19937 &gen) {
    std::uniform_int_distribution<> dis(0, 2);
    int block1 = dis(gen);
    int block2 = dis(gen);

    if (block1 != block2) {
        for (int col = 0; col < 3; ++col) {
            for (int row = 0; row < N; ++row) {
                std::swap(b[index_2d(row, block1 * 3 + col)], b[index_2d(row, block2 * 3 + col)]);
            }
        }
    }
}

static void permute_numbers(std::span<char> b, std::mt19937 &gen) {
    std::vector<char> permutation = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    std::shuffle(permutation.begin(), permutation.end(), gen);

    for (char &c : b) {
        if (c >= '1' && c <= '9') {
            c = permutation[c - '1'];
        }
    }
}

static bool is_solvable(std::string_view board_string) {
    board bd(board_string);
    auto solve_path = solve(bd, all_strategies);
    return !solve_path.empty();
}

std::string generate_complete_board(int seed) {
    std::string board_str = default_board_str;

    std::mt19937 gen(seed);
    for (int i = 0; i < 3; ++i) {
        swap_rows(board_str, i, gen);
        swap_columns(board_str, i, gen);
    }
    swap_row_blocks(board_str, gen);
    swap_column_blocks(board_str, gen);
    permute_numbers(board_str, gen);

    return board_str;
}

using index_quad = std::array<int, 4>;
using index_double = std::array<int, 2>;
using index_single = std::array<int, 1>;

static std::vector<index_quad> generate_quads() {
    std::vector<index_quad> quads;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 5; ++col) {
            quads.push_back({
                index_2d(row, col),
                index_2d(8 - col, row),
                index_2d(col, 8 - row),
                index_2d(8 - row, 8 - col),
            });
        }
    }
    return quads;
}

static std::vector<index_double> generate_doubles(std::string_view existing) {
    std::vector<index_double> doubles;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (existing[index_2d(row, col)] != '0') {
                doubles.push_back({
                    index_2d(row, col),
                    index_2d(8 - row, 8 - col),
                });
            }
        }
    }

    int row = 4;
    for (int col = 0; col < 5; ++col) {
        if (existing[index_2d(row, col)] != '0') {
            doubles.push_back({
                index_2d(row, col),
                index_2d(8 - col, 8 - row),
                });
        }
    }

    return doubles;
}

static std::vector<index_single> generate_singles(std::string_view existing) {
    std::vector<index_single> singles;
    for (int i = 0; i < 81; ++i) {
        if (existing[i] != '0') {
            singles.push_back({i});
        }
    }
    return singles;
}

template <size_t N>
static void safely_remove(std::string &bd, const std::vector<std::array<int, N>> &index_groups, int target_empties) {
    for (const auto &index_group : index_groups) {
        if (std::ranges::count(bd, '0') >= target_empties) {
            break;
        }

        // Copy the values to be removed
        std::array<char, N> copy{};
        for (int i = 0; i < N; ++i) {
            copy[i] = bd[index_group[i]];
        }

        // Remove the values
        // NOTE: this is done in two steps because if the index
        // group contains duplicates, the first removal will
        // clobber the copy making us unable to reset to a valid state.
        for (int i = 0; i < N; ++i) {
            bd[index_group[i]] = '0';
        }

        if (!is_unique(bd)) {
            for (int i = 0; i < N; ++i) {
                bd[index_group[i]] = copy[i];
            }
        }
    }
}

std::string puzzle_generator::generate_puzzle(std::string_view solution) {
    std::string board_str(solution);

    std::vector quads = generate_quads();
    std::ranges::shuffle(quads, gen);
    safely_remove(board_str, quads, 20);

    std::vector doubles = generate_doubles(board_str);
    std::ranges::shuffle(doubles, gen);
    safely_remove(board_str, doubles, 60);

    std::vector singles = generate_singles(board_str);
    std::ranges::shuffle(singles, gen);
    safely_remove(board_str, singles, 60);

    return board_str;
}

std::string puzzle_generator::generate_solution() {
    std::string board_str = default_board_str;

    for (int i = 0; i < 3; ++i) {
        swap_rows(board_str, i, gen);
        swap_columns(board_str, i, gen);
    }
    swap_row_blocks(board_str, gen);
    swap_column_blocks(board_str, gen);
    permute_numbers(board_str, gen);

    return board_str;
}


