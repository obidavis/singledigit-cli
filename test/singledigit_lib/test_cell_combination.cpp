//
// Created by Obi Davis on 30/08/2024.
//

#include "eliminations_matcher.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "board/constraint_set.hpp"
#include "strategies/cell_combination.hpp"

template <size_t N>
bool operator==(const cell_combination_elimination<N> &lhs, const cell_combination_elimination<N> &rhs) {
    return lhs.cells == rhs.cells
        && lhs.eliminated_values == rhs.eliminated_values
        && lhs.c_set == rhs.c_set
        && lhs.eliminated_cells == rhs.eliminated_cells;
}

TEST(CellCombinationTest, NakedPairs) {
    const board bd("0h2222167674g10981c0090590g10h033070c2g11109e260050h600941822194g190960h1105g18g8g0321410921820h41940990g116g111410m2m8109262682a209g176740hb636050ha212093041b2g1");

    std::vector elims = naked_pairs(bd);
    EXPECT_EQ(elims.size(), 5);
    naked_pair_elimination elim1(
        {C1},
        {1, 6},
        {A2, A3},
        {constraint_set_type::box, 0}
    );

    naked_pair_elimination elim2(
        {A4, A5, A6},
        {1, 6},
        {A2, A3},
        {constraint_set_type::row, 0}
    );

    naked_pair_elimination elim3(
        {C1, C5},
        {6, 7},
        {C6, C9},
        {constraint_set_type::row, 2}
    );

    naked_pair_elimination elim4(
        {D5, F5},
        {4, 8},
        {E4, E5},
        {constraint_set_type::box, 4}
    );

    naked_pair_elimination elim5(
        {D8, F9},
        {5, 8},
        {D7, F7},
        {constraint_set_type::box, 5}
    );

    EXPECT_THAT(elims, UnorderedElementsAreConcrete(elim1, elim2, elim3, elim4, elim5));
}

TEST(CellCombinationTest, NakedPairsApply) {
    board bd("0h2222167674g10981c0090590g10h033070c2g11109e260050h600941822194g190960h1105g18g8g0321410921820h41940990g116g111410m2m8109262682a209g176740hb636050ha212093041b2g1");

    naked_pair_elimination elim(
        {A2, A3},
        {1, 6},
        {C1},
        {constraint_set_type::box, 1}
    );

    elim.apply(bd);

    for (const cell &cell : bd[elim.eliminated_cells]) {
        value_set overlap = cell.candidates() & elim.eliminated_values;
        EXPECT_TRUE(overlap.empty());
    }
}
