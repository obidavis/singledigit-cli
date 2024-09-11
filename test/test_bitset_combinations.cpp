#include <gtest/gtest.h>
#include <bitset>
#include <gmock/internal/gmock-internal-utils.h>

#include "settools/bitset_combinations.hpp"

// Function to calculate the binomial coefficient (n choose k)
unsigned long long choose(unsigned long long n, unsigned long long k) {
    if (k > n) throw std::invalid_argument("k cannot be greater than n");
    if (k == 0 || n == k) return 1;

    unsigned long long result = 1;
    for (unsigned long long i = 1; i <= k; ++i) {
        result *= n--;
        result /= i;
    }
    return result;
}

// Test for next_bit_permutation function
TEST(BitPermutationTest, BasicPermutation) {
    // Test known bit patterns
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b0011), 0b0101);
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b0101), 0b0110);
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b0110), 0b1001);
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b1001), 0b1010);
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b1010), 0b1100);
}

TEST(BitPermutationTest, EdgeCaseZero) {
    // Edge case: next permutation of 0 should still be 0
    EXPECT_EQ(next_bit_permutation<uint8_t>(0), 0);
}

// Test for next_bit_permutation function with all unsigned integer sizes
TEST(BitPermutationTest, WorksWithAllSizes) {
    // Test with uint8_t
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b0011), 0b0101);
    EXPECT_EQ(next_bit_permutation<uint8_t>(0b1010), 0b1100);

    // Test with uint16_t
    EXPECT_EQ(next_bit_permutation<uint16_t>(0b00001111), 0b00010111);
    EXPECT_EQ(next_bit_permutation<uint16_t>(0b00110011), 0b00110101);

    // Test with uint32_t
    EXPECT_EQ(next_bit_permutation<uint32_t>(0b00000000000000000000000000001111), 0b00000000000000000000000000010111);
    EXPECT_EQ(next_bit_permutation<uint32_t>(0b00000000000000000000000000011011), 0b00000000000000000000000000011101);

    // Test with uint64_t
    EXPECT_EQ(next_bit_permutation<uint64_t>(0x000000000000000F), 0x0000000000000017);
    EXPECT_EQ(next_bit_permutation<uint64_t>(0x0000000000000F0F), 0x0000000000000F17);

    // Test with __uint128_t
    EXPECT_EQ(next_bit_permutation<__uint128_t>(0b0111), 0b1011);
    EXPECT_EQ(next_bit_permutation<__uint128_t>(0b1011), 0b1101);

    __uint128_t v = (__uint128_t)1 << 127 | (__uint128_t)1 << 65 | (__uint128_t)1 << 64 | (__uint128_t)1 << 63 | (__uint128_t)1 << 1;
    __uint128_t expected = (__uint128_t)1 << 127 | (__uint128_t)1 << 65 | (__uint128_t)1 << 64 | (__uint128_t)1 << 63 | (__uint128_t)1 << 2;
    EXPECT_EQ(next_bit_permutation<__uint128_t>(v), expected);
}

// Test for bitset_combinations_view class
TEST(BitsetCombinationsViewTest, BasicCombination) {
    std::bitset<4> bs("1101"); // bitset with 3 active bits
    int n = 2;
    bitset_combinations_view<4> combinations(bs, n);

    std::vector<std::bitset<4>> expected = {
        std::bitset<4>("0101"),
        std::bitset<4>("1001"),
        std::bitset<4>("1100")
    };

    std::vector<std::bitset<4>> result;
    for (auto comb : combinations) {
        result.push_back(comb);
    }

    EXPECT_EQ(result, expected);
}

TEST(BitsetCombinationsViewTest, SingleBitCombination) {
    std::bitset<4> bs("1001"); // 2 bits set
    int n = 1;
    bitset_combinations_view<4> combinations(bs, n);

    std::vector<std::bitset<4>> expected = {
        std::bitset<4>("0001"),
        std::bitset<4>("1000")
    };

    std::vector<std::bitset<4>> result;
    for (auto comb : combinations) {
        result.push_back(comb);
    }

    EXPECT_EQ(result, expected);
}

TEST(BitsetCombinationsViewTest, FullCombination) {
    std::bitset<4> bs("1111"); // all bits set
    int n = 3;
    bitset_combinations_view<4> combinations(bs, n);

    std::vector<std::bitset<4>> expected = {
        std::bitset<4>("0111"),
        std::bitset<4>("1011"),
        std::bitset<4>("1101"),
        std::bitset<4>("1110")
    };

    std::vector<std::bitset<4>> result;
    for (auto comb : combinations) {
        result.push_back(comb);
    }

    EXPECT_EQ(result, expected);
}

TEST(BitsetCombinationsViewTest, NoCombination) {
    std::bitset<4> bs("0000"); // no bits set
    int n = 2;
    EXPECT_THROW(bitset_combinations_view<4> combinations(bs, n), std::invalid_argument);
}

TEST(BitsetCombinationsViewTest, InvalidCombinationThrows) {
    std::bitset<4> bs("0010"); // only 1 bit set
    int n = 2;
    EXPECT_THROW(bitset_combinations_view<4> combinations(bs, n), std::invalid_argument);
}

TEST(BitsetCombinationsViewTest, ValidCombinationDoesNotThrow) {
    std::bitset<4> bs("1101"); // 3 bits set
    int n = 2;
    EXPECT_NO_THROW(bitset_combinations_view<4> combinations(bs, n));
}

// Test that n = 0 throws an exception
TEST(BitsetCombinationsViewTest, InvalidNZeroThrows) {
    std::bitset<4> bs("1111"); // any bitset
    int n = 0;
    EXPECT_THROW(bitset_combinations_view<4> combinations(bs, n), std::invalid_argument);
}


// Test for bitset_combinations_view with different sizes
TEST(BitsetCombinationsViewTest, WorksWithAllSizes) {
    // Test with uint8_t-sized bitset
    std::bitset<8> bs8("11001100");
    int n = 3;
    bitset_combinations_view<8> combinations8(bs8, n);
    std::vector<std::bitset<8>> result8;
    for (auto comb : combinations8) {
        result8.push_back(comb);
    }
    EXPECT_EQ(result8.size(), choose(bs8.count(), n)); // Check the expected number of combinations

    // Test with uint16_t-sized bitset
    std::bitset<16> bs16("1100110011001100");
    n = 4;
    bitset_combinations_view<16> combinations16(bs16, n);
    std::vector<std::bitset<16>> result16;
    for (auto comb : combinations16) {
        result16.push_back(comb);
    }
    EXPECT_EQ(result16.size(), choose(bs16.count(), n)); // Check the expected number of combinations

    // Test with uint32_t-sized bitset
    std::bitset<32> bs32("11001100110011001100110011001100");
    n = 5;
    bitset_combinations_view<32> combinations32(bs32, n);
    std::vector<std::bitset<32>> result32;
    for (auto comb : combinations32) {
        result32.push_back(comb);
    }
    EXPECT_EQ(result32.size(), choose(bs32.count(), n));

    // Test with uint64_t-sized bitset
    std::bitset<64> bs64("1100110011001100110011001100110011001100110011001100110011001100");
    n = 6;
    bitset_combinations_view<64> combinations64(bs64, n);
    std::vector<std::bitset<64>> result64;
    for (auto comb : combinations64) {
        result64.push_back(comb);
    }
    EXPECT_EQ(result64.size(), choose(bs64.count(), n)); // Check the expected number of combinations

    // Test with __uint128_t-sized bitset (requires manual construction)
    std::bitset<128> bs128;
    for (int i = 0; i < 128; i += 2) { // Set alternating bits in the 128-bit number
        bs128.set(i);
    }
    n = 2;
    bitset_combinations_view<128> combinations128(bs128, n);
    std::vector<std::bitset<128>> result128;
    for (auto comb : combinations128) {
        result128.push_back(comb);
    }
    for (auto comb : result128) {
        EXPECT_EQ(comb.count(), n) << "Combination " << comb << " does not have " << n << " bits set";
    }
    EXPECT_EQ(result128.size(), choose(bs128.count(), n)); // Check the expected number of combinations
}