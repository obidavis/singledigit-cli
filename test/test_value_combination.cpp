//
// Created by Obi Davis on 28/08/2024.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "strategies/value_combination.hpp"
#include "board/board.hpp"


TEST(ValueCombinationTest, HiddenPairs) {
    /*  Sample from sudokuwiki:
     *  https://www.sudokuwiki.org/sudoku.htm?bd=4105300hg281j209i2j081381ag614j20h410hh80318412181h00581033k4109g130342gi0k86s811103m8i4igh0l85805210hla81g20550g12181500h0309090h50120654i0i081032181g10h09054111
     *  Hidden Pair
        Hidden Pair 2/4 in Col 3, on cells [D3,E3]
        - removes 5/6 from D3
        - removes 3/6/7 from E3
        Hidden Pair 2/4 in Box 4, on cells [D3,E3]
        - removes 5/6 from D3
        - removes 3/6/7 from E3
        Hidden Pair 3/7 in Box 6, on cells [E7,F7]
        - removes 6/9 from E7
        - removes 1/5/9 from F7
        Hidden Pair 3/7 in Col 7, on cells [E7,F7]
        - removes 6/9 from E7
        - removes 1/5/9 from F7
     */

    board bd("4105300hg281j209i2j081381ag614j20h410hh80318412181h00581033k4109g130342gi0k86s811103m8i4igh0l85805210hla81g20550g12181500h0309090h50120654i0i081032181g10h09054111");

    std::vector elims = hidden_pairs(bd);

    value_combination_elimination elim1 = {
        .cells = bd.cells_at({D3, E3}),
        .values = {2, 4},
        .c_set = bd.col(3)
    };

    value_combination_elimination elim2 = {
        .cells = bd.cells_at({D3, E3}),
        .values = {2, 4},
        .c_set = bd.box(4)
    };

    value_combination_elimination elim3 = {
        .cells = bd.cells_at({E7, F7}),
        .values = {3, 7},
        .c_set = bd.col(7)
    };

    value_combination_elimination elim4 = {
        .cells = bd.cells_at({E7, F7}),
        .values = {3, 7},
        .c_set = bd.box(6)
    };

    EXPECT_THAT(elims, testing::UnorderedElementsAre(elim1, elim2, elim3, elim4));
}
