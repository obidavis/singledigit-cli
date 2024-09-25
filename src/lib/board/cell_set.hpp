#ifndef CELL_SET_HPP
#define CELL_SET_HPP

#include <bitset>
#include <ranges>
#include "detail/bitset_combinations.hpp"
#include "cell.hpp"

class cell_set {
public:
    cell_set(std::initializer_list<cell_index> indices) {
        for (cell_index index : indices) {
            mask.set(index, true);
        }
    }
    cell_set() = default;
    cell_set(const cell_set &) = default;
    cell_set(cell_set &&) = default;
    cell_set &operator=(const cell_set &) = default;
    cell_set &operator=(cell_set &&) = default;

    [[nodiscard]] bool at(int index) const {
        return mask[index];
    }

    void set(int index, bool value) {
        mask.set(index, value);
    }

    [[nodiscard]] bool empty() const {
        return mask.none();
    }

    [[nodiscard]] size_t count() const {
        return mask.count();
    }

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] bool operator==(const cell_set &rhs) const {
        return mask == rhs.mask;
    }

    cell_set operator|(const cell_set &rhs) const {
        return cell_set{mask | rhs.mask};
    }

    cell_set operator&(const cell_set &rhs) const {
        return cell_set{mask & rhs.mask};
    }

    cell_set operator-(const cell_set &rhs) const {
        return cell_set{mask & ~rhs.mask};
    }

    cell_set &operator|=(const cell_set &rhs) {
        mask |= rhs.mask;
        return *this;
    }

    friend auto combinations(const cell_set &cs, int n) {
        return std::views::transform(bitset_combinations_view<81>(cs.mask, n), [](auto &&bs) {
            return cell_set{bs};
        });
    }

protected:
    explicit cell_set(std::bitset<81> mask) : mask(mask) {}
    std::bitset<81> mask;
};

#endif //CELL_SET_HPP
