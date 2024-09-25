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
        Hidden Pair 2/4 in Col 3, on eliminated_cells [D3,E3]
        - removes 5/6 from D3
        - removes 3/6/7 from E3
        Hidden Pair 2/4 in Box 4, on eliminated_cells [D3,E3]
        - removes 5/6 from D3
        - removes 3/6/7 from E3
        Hidden Pair 3/7 in Box 6, on eliminated_cells [E7,F7]
        - removes 6/9 from E7
        - removes 1/5/9 from F7
        Hidden Pair 3/7 in Col 7, on eliminated_cells [E7,F7]
        - removes 6/9 from E7
        - removes 1/5/9 from F7
     */

    const board bd("4105300hg281j209i2j081381ag614j20h410hh80318412181h00581033k4109g130342gi0k86s811103m8i4igh0l85805210hla81g20550g12181500h0309090h50120654i0i081032181g10h09054111");

    std::vector elims = hidden_pairs(bd);

    value_combination_elimination elim1 = {
        .cells = {D3, E3},
        .values = {2, 4},
        .c_set = {constraint_set_type::column, 2}
    };

    value_combination_elimination elim2 = {
        .cells = {D3, E3},
        .values = {2, 4},
        .c_set = {constraint_set_type::box, 3}
    };

    value_combination_elimination elim3 = {
        .cells = {E7, F7},
        .values = {3, 7},
        .c_set = {constraint_set_type::column, 6}
    };

    value_combination_elimination elim4 = {
        .cells = {E7, F7},
        .values = {3, 7},
        .c_set = {constraint_set_type::box, 5}
    };

    std::vector<value_combination_elimination> actual_elims;
    std::ranges::transform(elims, std::back_inserter(actual_elims), [](const elimination &elim) {
        return std::get<value_combination_elimination>(elim);
    });

    EXPECT_THAT(actual_elims, testing::UnorderedElementsAre(elim1, elim2, elim3, elim4));
}

TEST(ValueCombinationTest, HiddenPairsApply) {
    board bd("4105300hg281j209i2j081381ag614j20h410hh80318412181h00581033k4109g130342gi0k86s811103m8i4igh0l85805210hla81g20550g12181500h0309090h50120654i0i081032181g10h09054111");
    value_combination_elimination elim1 = {
        .cells = {D3, E3},
        .values = {2, 4},
        .c_set = constraint_set{constraint_set_type::column, 3}
    };
    elim1.apply(bd);
    for (const cell &c : bd[elim1.cells]) {
        value_set all = c.candidates() | elim1.values;
        EXPECT_EQ(all, elim1.values);
    }
}
