#include <solver.hpp>
#include <uniqueness.hpp>
#include <future>
#include <algorithm>
#include <ranges>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "generator.hpp"
#include "board/board.hpp"


// TEST(TestGenerate, Valid) {
//     // std::string solution = generate_complete_board(10);
//     puzzle_generator gen(10);
//     for (int i = 0; i < 25; ++i) {
//         std::string puzzle = gen.generate_puzzle(solution);
//         board bd(puzzle);
//         EXPECT_TRUE(bd.is_valid());
//         std::vector path = solve(bd, all_strategies);
//         // if (path.empty()) {
//         //     std::string message = "Puzzle is not solvable:\n" + puzzle;
//         //     FAIL() << message;
//         // }
//         EXPECT_TRUE(is_unique(puzzle));
//     }
// }
//
// TEST(TestGenerate, ThreadDeterminism) {
//     const int num_puzzles = 50;
//     const int num_threads = 5;
//     const unsigned int seed = 0;
//
//
//     thread_safe_seed_generator async_gen(seed);
//     auto generate_task = [&](size_t count) {
//         std::vector<std::string> results;
//         for (size_t i = 0; i < count; ++i) {
//             puzzle_generator generator(async_gen());
//             std::string solution = generator.generate_solution();
//             std::string puzzle = generator.generate_puzzle(solution);
//             results.push_back(puzzle);
//         }
//         return results;
//     };
//     std::vector<std::future<std::vector<std::string>>> futures;
//     futures.reserve(num_threads);
//     const size_t num_puzzles_per_thread = num_puzzles / num_threads;
//     for (int i = 0; i < num_threads; ++i) {
//         futures.push_back(std::async(std::launch::async, generate_task, num_puzzles_per_thread));
//     }
//
//     std::vector<std::string> async_puzzles;
//     async_puzzles.reserve(num_puzzles);
//     for (auto &f : futures) {
//         std::vector result = f.get();
//         async_puzzles.insert(async_puzzles.end(), result.begin(), result.end());
//     }
//
//     thread_safe_seed_generator sync_gen(seed);
//     std::vector<std::string> sync_puzzles;
//     for (int i = 0; i < num_puzzles; ++i) {
//         puzzle_generator generator(sync_gen());
//         std::string solution = generator.generate_solution();
//         std::string puzzle = generator.generate_puzzle(solution);
//         sync_puzzles.push_back(puzzle);
//     }
//
//     std::sort(async_puzzles.begin(), async_puzzles.end());
//     std::sort(sync_puzzles.begin(), sync_puzzles.end());
//     EXPECT_THAT(async_puzzles, testing::ElementsAreArray(sync_puzzles));
//
// }
