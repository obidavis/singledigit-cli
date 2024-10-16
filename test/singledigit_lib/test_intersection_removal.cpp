//
// Created by Obi Davis on 09/09/2024.
//

#include "eliminations_matcher.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "strategies/intersection_removal.hpp"

bool operator==(const intersection_removal_elimination &lhs, const intersection_removal_elimination &rhs) {
    return lhs.intersection == rhs.intersection &&
           lhs.eliminated_cells == rhs.eliminated_cells &&
           lhs.eliminated_values == rhs.eliminated_values &&
           lhs.c_sets == rhs.c_sets;
}

TEST(IntersectionRemoval, PointingPairsTriples) {
    board bd("k00905cgdg2103hgc00h03j0ccd81ck8h8e8608130g15o034s1s6c11448222g1482c8c0hg421gg8o9o1ogc4103094g8222054gi0o01126g128112a814s0s4c810k2o4c680c1103g10611410s0qg181210c");
    std::vector results = pointing_pairs_triples(bd);

    // };
    intersection_removal_elimination elim = {
        {E7},
        {9},
        {E1, E3},
        {{constraint_set_type::box, 3}, {constraint_set_type::row, 4}}
    };

    EXPECT_THAT(results, UnorderedElementsAreConcrete(elim));
}