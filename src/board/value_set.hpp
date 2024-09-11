#ifndef VALUE_SET_HPP
#define VALUE_SET_HPP

#include <bitset>
#include <ranges>

#include "detail/bitset_combinations.hpp"

struct value_set {
    value_set(std::initializer_list<int> list);
    static value_set all();
    static value_set none();
    static value_set from_uint(uint32_t v);

    [[nodiscard]] std::string to_string() const;
    [[nodiscard]] size_t count() const;
    [[nodiscard]] int first() const;
    [[nodiscard]] bool empty() const;

    void set(int i);
    void set(int i, bool value);
    [[nodiscard]] bool at(int i) const;

    [[nodiscard]] bool operator==(const value_set &rhs) const;
    [[nodiscard]] bool operator!=(const value_set &rhs) const;

    value_set operator|(const value_set &rhs) const;
    value_set operator&(const value_set &rhs) const;
    value_set operator-(const value_set &rhs) const;
    value_set operator^(const value_set &rhs) const;
    value_set operator~() const;

    value_set &operator|=(const value_set &rhs);
    value_set &operator&=(const value_set &rhs);
    value_set &operator-=(const value_set &rhs);
    value_set &operator^=(const value_set &rhs);

    [[nodiscard]] operator bool() const;

    friend auto combinations(const value_set &vs, int n) {
        return std::views::transform(bitset_combinations_view<10>(vs.values, n), [](auto &&bs) {
            return value_set{bs};
        });
    }
private:
    value_set(std::bitset<10> values) : values(values) {}
    std::bitset<10> values;

};

#endif //VALUE_SET_HPP
