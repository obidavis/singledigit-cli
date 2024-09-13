// #include "gtest/gtest.h"
// #include "board/cell_set.hpp"
//
// class CellSetTest : public ::testing::Test {
// protected:
//     std::array<cell, 81> cells;
//     std::bitset<81> mask;
//
//     void SetUp() override {
//         for (int i = 0; i < 81; i++) {
//             if (i % 2 == 0) {
//                 cells[i] = cell(cell_index{i}, 1);
//             } else {
//                 cells[i] = cell(cell_index{i});
//             }
//         }
//         mask.set(1);
//         mask.set(2);
//         mask.set(5);
//     }
// };
//
//
// TEST_F(CellSetTest, TestWhere) {
//     cell_set set(cells, mask);
//     auto all = set.where([](const cell &cell) {
//         return true;
//     });
//     ASSERT_EQ(all.count(), 3);
//
//     auto none = set.where([](const cell &cell) {
//         return false;
//     });
//     ASSERT_EQ(none.count(), 0);
//
//     auto solved = set.where([](const cell &cell) {
//         return cell.is_solved() && cell.value() == 1;
//     });
//     ASSERT_EQ(solved.count(), 1);
// }
//
// TEST_F(CellSetTest, TestIterators) {
//     cell_set set(cells, mask);
//     auto it = set.begin();
//     ASSERT_FALSE((*it).is_solved());
//     ASSERT_NE(it, set.end());
//     ++it;
//     ASSERT_TRUE((*it).is_solved());
//     ASSERT_NE(it, set.end());
//     ++it;
//     ASSERT_FALSE((*it).is_solved());
//     ASSERT_NE(it, set.end());
//     ++it;
//     ASSERT_EQ(it, set.end());
// }
//
// TEST_F(CellSetTest, TestCombinations) {
//     cell_set set(cells, mask);
//     auto combs = combinations(set, 2);
//     std::vector<cell_set> as_vector(combs.begin(), combs.end());
//     ASSERT_EQ(as_vector.size(), 3);
//     ASSERT_EQ(as_vector[0].count(), 2);
//     ASSERT_EQ(as_vector[1].count(), 2);
//     ASSERT_EQ(as_vector[2].count(), 2);
// }
//
// TEST_F(CellSetTest, TestToString) {
//     cell_set set(cells, mask);
//     std::string expected = "{A2, A3, A6}";
//     ASSERT_EQ(set.to_string(), expected);
// }