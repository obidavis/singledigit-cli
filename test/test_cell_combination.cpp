//
// Created by Obi Davis on 30/08/2024.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "strategies/cell_combination.hpp"

TEST(CellCombinationTest, NakedPairs) {
    const board bd("0h2222167674g10981c0090590g10h033070c2g11109e260050h600941822194g190960h1105g18g8g0321410921820h41940990g116g111410m2m8109262682a209g176740hb636050ha212093041b2g1");

    std::vector elims = naked_pairs(bd);
    EXPECT_EQ(elims.size(), 5);
    cell_combination_elimination elim1 = {
        .cells = {A2, A3},
        .values = {1, 6},
        .c_set = bd.box(1),
        .eliminated_cells = {C1}
    };

    cell_combination_elimination elim2 = {
        .cells = {A2, A3},
        .values = {1, 6},
        .c_set = bd.row(1),
        .eliminated_cells = {A4, A5, A6}
    };

    cell_combination_elimination elim3 = {
        .cells = {C6, C9},
        .values = {6, 7},
        .c_set = bd.row(3),
        .eliminated_cells = {C1, C5}
    };

    cell_combination_elimination elim4 = {
        .cells = {E4, E5},
        .values = {4, 8},
        .c_set = bd.box(5),
        .eliminated_cells = {D5, F5}
    };

    cell_combination_elimination elim5 = {
        .cells = {D7, F7},
        .values = {5, 8},
        .c_set = bd.box(6),
        .eliminated_cells = {D8, F9}
    };

    std::vector<cell_combination_elimination> actual_elims;
    std::ranges::transform(elims, std::back_inserter(actual_elims), [](const elimination &elim) {
        return elim.get<cell_combination_elimination>();
    });
    EXPECT_THAT(actual_elims, testing::UnorderedElementsAre(elim1, elim2, elim3, elim4, elim5));
}

TEST(CellCombinationTest, ApplyElimination) {
    board bd("0h2222167674g10981c0090590g10h033070c2g11109e260050h600941822194g190960h1105g18g8g0321410921820h41940990g116g111410m2m8109262682a209g176740hb636050ha212093041b2g1");

    cell_combination_elimination elim = {
        .cells = {A2, A3},
        .values = {1, 6},
        .c_set = bd.box(1),
        .eliminated_cells = {C1}
    };

    elim.apply(bd);

    for (const cell &cell : bd[elim.eliminated_cells]) {
        value_set overlap = cell.candidates() & elim.values;
        EXPECT_TRUE(overlap.empty());
    }
}
