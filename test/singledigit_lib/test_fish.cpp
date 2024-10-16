//
// Created by Obi Davis on 30/09/2024.
//

#include "strategies/fish.hpp"
#include "gtest/gtest.h"

TEST(Fish, XWing) {
    board bd("03c848csc4cs1121g10hg005481020024881c8112002c0g1040h485848g0210h4481140350200gs403c4k81448050281k0091120k00gc80h4811s4cck80320g1c810c820020hc805210503cos0cok8s811");
    auto eliminations = x_wing(bd);
    ASSERT_EQ(eliminations.size(), 1);
    auto x_wing_elim = dynamic_cast<fish_elimination<2> *>(eliminations[0].get());
    ASSERT_NE(x_wing_elim, nullptr);
}

TEST(Fish, Swordfish) {
    board bd("g104209o8gd80250d0100941q003q00h05o0810h02h4h0l421l0090410g040090h81032040020gp021p0h408h40821810305h0l00gl003k00528h028l0810h0g8111g441020921g420k008pk8gp4l4l003");
    auto eliminations = swordfish(bd);
    ASSERT_EQ(eliminations.size(), 1);
    auto swordfish_elim = dynamic_cast<fish_elimination<3> *>(eliminations[0].get());
    ASSERT_NE(swordfish_elim, nullptr);
}
