//
// Created by Obi Davis on 28/08/2024.
//

#ifndef COMBINATIONS_HPP
#define COMBINATIONS_HPP

// http://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
template<std::unsigned_integral T>
static constexpr T next_bit_permutation(T v) {
    T t = v | (v - 1);
    return (t + 1) | (((~t & -~t) - 1) >> (std::countr_zero(v) + 1));
}

template <size_t N>
struct uintN {
    static consteval auto get_type() {
        if constexpr (N <= 8) {
            return uint8_t{};
        } else if constexpr (N <= 16) {
            return uint16_t{};
        } else if constexpr (N <= 32) {
            return uint32_t{};
        } else if constexpr (N <= 64) {
            return uint64_t{};
        } else {
            return __uint128_t{};
        }
    }
    using type = decltype(get_type());
};

template <size_t N>
using uintN_t = typename uintN<N>::type;

template <size_t N>
static constexpr auto one = uintN_t<N>{1};

template <size_t N>
struct bitset_combinations_view {
    bitset_combinations_view(const std::bitset<N> &bs, int n) : bs(bs), n(n) {
        if (n <= 0) {
            throw std::invalid_argument("n must be greater than 0");
        }
        if (bs.count() < n) {
            this->bs.reset();
        }
    }
    struct iterator {
        using value_type = std::bitset<N>;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::bitset<N>*;
        using reference = const std::bitset<N>&;
        using iterator_category = std::forward_iterator_tag;

        iterator() = default;
        iterator(std::bitset<N> bs, int n, bool end) : k(bs.count()) {

            for (size_t i = 0, j = 0; i < N; i++) {
                if (bs.test(i)) {
                    bit_indices[j++] = i;
                }
            }
            if (end) {
                // past-the-end combination
                // kth bit = 1; lowest n-1 bits = 1
                bit_pattern = (one<N> << k) | ((one<N> << (n - 1)) - 1);
            } else {
                // first combination
                // lowest n bits = 1
                bit_pattern = (one<N> << n) - 1;
            }

            // if no bits set, all iterators == 0
            bit_pattern *= (k > 0);
        }

        bool operator==(const iterator &rhs) const {
            return bit_pattern == rhs.bit_pattern;
        }
        bool operator!=(const iterator &rhs) const {
            return bit_pattern != rhs.bit_pattern;
        }
        iterator& operator++() {
            bit_pattern = next_bit_permutation(bit_pattern);
            return *this;
        }

        iterator operator++(int) {
            iterator copy = *this;
            ++(*this);
            return copy;
        }

        std::bitset<N> operator*() const {
            std::bitset<N> result{};
            for (size_t i = 0; i < k; ++i) {
                result.set(bit_indices[i], bit_pattern & (one<N> << i));
            }
            return result;
        }

        size_t k{};
        uintN_t<N> bit_pattern;
        uint8_t bit_indices[N]{};
    };

    iterator begin() {
        return {bs, n, false};
    }
    iterator end() {
        return {bs, n, true};
    }

private:
    std::bitset<N> bs;
    int n;
};

static_assert(std::ranges::range<bitset_combinations_view<10>>);

#endif //COMBINATIONS_HPP
