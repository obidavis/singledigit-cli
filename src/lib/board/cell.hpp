//
// Created by Obi Davis on 28/08/2024.
//

#ifndef CELL_HPP
#define CELL_HPP

#include "value_set.hpp"

enum cell_index : int {
    A1, A2, A3, A4, A5, A6, A7, A8, A9,
    B1, B2, B3, B4, B5, B6, B7, B8, B9,
    C1, C2, C3, C4, C5, C6, C7, C8, C9,
    D1, D2, D3, D4, D5, D6, D7, D8, D9,
    E1, E2, E3, E4, E5, E6, E7, E8, E9,
    F1, F2, F3, F4, F5, F6, F7, F8, F9,
    G1, G2, G3, G4, G5, G6, G7, G8, G9,
    H1, H2, H3, H4, H5, H6, H7, H8, H9,
    I1, I2, I3, I4, I5, I6, I7, I8, I9
};

struct cell {
    cell(cell_index index);
    explicit cell(cell_index index, int value);
    explicit cell(cell_index index, value_set candidates);

    void solve(int value);
    int remove_candidate(int value);
    int remove_candidates(value_set values);
    [[nodiscard]] int value() const;
    [[nodiscard]] value_set candidates() const;
    [[nodiscard]] bool is_solved() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] cell_index index() const;
private:
    cell_index _index;
    value_set _candidates;
    bool _solved;
};


inline std::string index_to_string(cell_index index) {
    char row = 'A' + static_cast<int>(index) / 9;
    char col = '1' + static_cast<int>(index) % 9;
    return {row, col};
}

#endif //CELL_HPP
