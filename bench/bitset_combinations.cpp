//
// Created by Obi Davis on 16/10/2024.
//

#include "board/detail/bitset_combinations.hpp"

#include "benchmark/benchmark.h"
#include "generator.hpp"
#include <random>
#include <bitset>

static void BM_bitset_combinations(benchmark::State &state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> position_dis(0, 80);
    std::uniform_int_distribution<> count_dis(15, 50);
    std::array<std::vector<int>, 1000> positions;
    for (auto &v : positions) {
        size_t count = count_dis(gen);
        v.reserve(count);
        for (int i = 0; i < count; i++) {
            v.push_back(position_dis(gen));
        }
    }
    int i = 0;
    for (auto _ : state) {
        std::bitset<81> bs{};
        const std::vector<int> &v = positions[i++ % 1000];
        for (int pos : v) {
            bs.set(pos);
        }
        for (auto &&bs : bitset_combinations_view(bs, 3)) {
            benchmark::DoNotOptimize(bs);
        }
    }
}

BENCHMARK(BM_bitset_combinations);

BENCHMARK_MAIN();

