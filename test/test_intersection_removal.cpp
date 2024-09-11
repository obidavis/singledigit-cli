//
// Created by Obi Davis on 09/09/2024.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "strategies/intersection_removal.hpp"

TEST(IntersectionRemoval, PointingPairsTriples) {
    board bd("k00905cgdg2103hgc00h03j0ccd81ck8h8e8608130g15o034s1s6c11448222g1482c8c0hg421gg8o9o1ogc4103094g8222054gi0o01126g128112a814s0s4c810k2o4c680c1103g10611410s0qg181210c");
    std::vector results = pointing_pairs_triples(bd);

    intersection_removal_elimination elim {
        .intersection = bd.cells_at({E1, E3}),
        .eliminated_cells = bd.cells_at({E7}),
        .eliminated_values = {9},
        .c_sets = {bd.box(4), bd.row(5)}
    };

    EXPECT_THAT(results, testing::UnorderedElementsAre(elim));
}