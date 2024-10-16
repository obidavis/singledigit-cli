//
// Created by Obi Davis on 28/08/2024.
//

#ifndef COMBINATIONS_HPP
#define COMBINATIONS_HPP

#include <vector>
#include <algorithm>
#include <bitset>

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
        iterator(std::bitset<N> bs, int n, bool end) : k(bs.count()), bits(k), done(end) {
            for (size_t i = 0, j = 0; i < N; i++) {
                if (bs.test(i)) {
                    bit_indices[j++] = i;
                }
            }
            if (n < k) {
                if (end) {
                    std::fill_n(bits.rbegin(), n, true);
                } else {
                    std::fill_n(bits.begin(), n, true);
                }
            } else {
                done = true;
            }
        }

        bool operator==(const iterator &rhs) const {
            return done;
        }
        bool operator!=(const iterator &rhs) const {
            return !done;
        }
        iterator& operator++() {
            done = !std::prev_permutation(bits.begin(), bits.end());
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
                result.set(bit_indices[i], bits[i]);
            }
            return result;
        }

        size_t k{};
        std::vector<bool> bits;
        uint8_t bit_indices[N]{};
        bool done;
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
