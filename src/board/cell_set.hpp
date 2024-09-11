#ifndef CELL_SET_HPP
#define CELL_SET_HPP

#include <span>
#include <bitset>
#include <numeric>
#include <string>
#include <ranges>
#include <cassert>

#include "detail/bitset_combinations.hpp"
#include "cell.hpp"
#include "value_set.hpp"

template <bool is_const>
class cell_set_base {
    using cell_span = std::span<std::conditional_t<is_const, const cell, cell>, 81>;
public:
    cell_set_base(cell_span cells, std::bitset<81> mask) : cells(cells), mask(mask) {}

    static cell_set_base all(std::span<cell, 81> span);
    static cell_set_base none(std::span<cell, 81> span);

    [[nodiscard]] int count() const;
    [[nodiscard]] bool empty() const;

    [[nodiscard]] cell_set_base where(std::predicate<const cell &> auto &&predicate) const;

    [[nodiscard]] cell_set_base open_cells() const;
    [[nodiscard]] cell_set_base closed_cells() const;
    [[nodiscard]] value_set open_values() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] cell_set_base operator-(const cell_set_base &rhs) const;
    [[nodiscard]] cell_set_base operator&(const cell_set_base &rhs) const;

    [[nodiscard]] bool operator==(const cell_set_base &other) const;
    [[nodiscard]] bool operator!=(const cell_set_base &other) const;

    struct iterator {
        using value_type = std::conditional_t<is_const, const cell, cell>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;

        reference operator*() {
            int index = std::countr_zero(mask);
            return cells[index];
        }

        iterator &operator++() {
            mask &= mask - __uint128_t{1ull};
            return *this;
        }

        iterator operator++(int) {
            auto copy = *this;
            ++*this;
            return copy;
        }

        bool operator==(const iterator &other) const = default;
        bool operator!=(const iterator &other) const = default;

        pointer cells{};
        __uint128_t mask{};
    };


    iterator begin() {
        return {cells.data(), std::bit_cast<__uint128_t>(mask)};
    }

    iterator end() {
        return {cells.data(), 0};
    }

    iterator begin() const {
        return {cells.data(), std::bit_cast<__uint128_t>(mask)};
    }

    iterator end() const {
        return {cells.data(), 0};
    }


    friend auto combinations(const cell_set_base &cs, int n) {
        return std::views::transform(bitset_combinations_view<81>(cs.mask, n), [cs](auto &&bs) {
            return cell_set_base{cs.cells, bs};
        });
    }

    template <typename T>
    struct indexed_view {
        struct iterator {

            using value_type = std::pair<cell_index, T &>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;
            using iterator_category = std::forward_iterator_tag;

            std::pair<cell_index, T &> operator*() const {
                int index = std::countr_zero(std::bit_cast<__uint128_t>(mask));
                return {cell_index{index}, cells[index]};
            }

            iterator &operator++() {
                int rightmost = std::countr_zero(std::bit_cast<__uint128_t>(mask));
                mask.set(rightmost, false);
                return *this;
            }

            iterator operator++(int) {
                auto copy = *this;
                ++*this;
                return copy;
            }

            bool operator==(const iterator &other) const {
                return mask.none();
            }

            bool operator!=(const iterator &other) const {
                return mask.any();
            }

            T *cells;
            std::bitset<81> mask;
        };

        iterator begin() {
            return {span.data(), mask};
        }

        iterator end() {
            return {span.data(), std::bitset<81>{}};
        }

        iterator begin() const {
            return {span.data(), mask};
        }

        iterator end() const {
            return {span.data(), std::bitset<81>{}};
        }

        std::span<T, 81> span;
        std::bitset<81> mask;
    };

    indexed_view<cell> indexed_values() requires (!is_const) {
        return {cells, mask};
    }

    indexed_view<const cell> indexed_values() const {
        return {cells, mask};
    }


private:
    cell_span cells;
    std::bitset<81> mask;
};

cell_set_base(std::span<cell, 81>, std::bitset<81>) -> cell_set_base<false>;
cell_set_base(std::span<const cell, 81>, std::bitset<81>) -> cell_set_base<true>;

using cell_set = cell_set_base<true>;
using mutable_cell_set = cell_set_base<false>;


template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::all(std::span<cell, 81> span) {
    return {span, std::bitset<81>().set()};
}

template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::none(std::span<cell, 81> span) {
    return {span, std::bitset<81>().reset()};
}

template<bool is_const>
int cell_set_base<is_const>::count() const {
    return static_cast<int>(mask.count());
}

template<bool is_const>
bool cell_set_base<is_const>::empty() const {
    return mask.none();
}

template <bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::where(std::predicate<const cell &> auto &&predicate) const {
    std::bitset<81> new_mask = mask; // Copy the mask
    for (size_t i = 0; i < 81; ++i) {
        new_mask.set(i, mask[i] && predicate(cells[i]));
    }
    return {cells, new_mask};
}

template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::open_cells() const {
    return where([](const cell &cell) {
        return !cell.is_solved();
    });
}

template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::closed_cells() const {
    return where([](const cell &cell) {
        return cell.is_solved();
    });
}

template<bool is_const>
value_set cell_set_base<is_const>::open_values() const {
    value_set values{};
    auto open = open_cells();
    for (const cell &cell : open) {
        values |= cell.candidates();
    }
    return values;
}

template<bool is_const>
std::string cell_set_base<is_const>::to_string() const {
    std::string result;
    size_t total_size = 2;              // For the braces
    total_size += count() * 2;          // For the cell values
    total_size += (count() - 1) * 2;    // For the commas and spaces
    result.reserve(total_size);

    auto indexed = indexed_values();
    result = std::accumulate(indexed.begin(), indexed.end(), std::string{"{"},
        [](const std::string &a, const auto &pair) {
            int index = pair.first;
            char row = 'A' + index / 9;
            char col = '1' + index % 9;
            std::string cell_index = {row, col};
            return a.empty() || a.back() == '{' ? a + cell_index : a + ", " + cell_index;
        });
    result += "}";
    return result;
}

template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::operator-(const cell_set_base &rhs) const {
    return {cells, mask & ~rhs.mask};
}

template<bool is_const>
cell_set_base<is_const> cell_set_base<is_const>::operator&(const cell_set_base &rhs) const {
    return {cells, mask & rhs.mask};
}

template<bool is_const>
bool cell_set_base<is_const>::operator==(const cell_set_base &other) const {
    assert(cells.data() == other.cells.data());
    return mask == other.mask;
}

template<bool is_const>
bool cell_set_base<is_const>::operator!=(const cell_set_base &other) const {
    return !(*this == other);
}

#endif //CELL_SET_HPP
