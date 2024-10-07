//
// Created by Obi Davis on 13/09/2024.
//

#ifndef CELLS_VIEW_HPP
#define CELLS_VIEW_HPP

#include "cell_set.hpp"

template <typename CellType>
struct cells_view {
    cells_view(CellType *cells, const cell_set &mask) : cells(cells), mask(mask) {}

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = CellType;
        using difference_type = std::ptrdiff_t;
        using pointer = CellType *;
        using reference = CellType &;

        iterator() : cells{}, mask{} {}
        iterator(CellType *cells, const cell_set &mask) : cells(cells), mask(std::bit_cast<__uint128_t>(mask)) {}

        CellType &operator*() const {
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

        bool operator==(const iterator &rhs) const {
            return mask == rhs.mask;
        }

        bool operator!=(const iterator &rhs) const {
            return mask != rhs.mask;
        }

        CellType *cells;
        __uint128_t mask;
    };

    iterator begin() {
        return iterator(cells, mask);
    }

    iterator end() {
        return iterator(cells, {});
    }

    cell_set where(std::predicate<const cell &> auto &&pred) const {
        cell_set result;
        for (int i = 0; i < 81; i++) {
            if (mask.at(i)) {
                if (pred(cells[i])) {
                    result.set(i, true);
                }
            }
        }
        return result;
    }

    [[nodiscard]] cell_set open_cells() const {
        return where([](const cell &c) { return !c.is_solved(); });
    }

    [[nodiscard]] cell_set solved_cells() const {
        return where([](const cell &c) { return c.is_solved(); });
    }

    [[nodiscard]] value_set open_values() const {
        value_set result = value_set::none();
        for (int i = 0; i < 81; ++i) {
            if (mask.at(i) && !cells[i].is_solved()) {
                result |= cells[i].candidates();
            }
        }
        return result;
    }

    [[nodiscard]] cell_set containing(value_set vs) {
        return where([vs](const cell &c) {
            return (c.candidates() & vs) == vs;
        });
    }

    [[nodiscard]] cell_set containing(int value) {
        return where([value](const cell &c) {
            return c.candidates().at(value);
        });
    }
private:
    CellType *cells;
    cell_set mask;
};

cells_view(const cell *cells, const cell_set &mask) -> cells_view<const cell>;
cells_view(cell *cells, const cell_set &mask) -> cells_view<cell>;

static_assert(std::ranges::input_range<cells_view<cell>>);
static_assert(std::ranges::input_range<cells_view<const cell>>);

#endif //CELLS_VIEW_HPP
