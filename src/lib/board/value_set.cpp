#include <bit>
#include <sstream>

#include "value_set.hpp"

static constexpr unsigned long n_choose_k(unsigned n, unsigned k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1;

    unsigned long result = n;
    for (int i = 2; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}


value_set::value_set(std::initializer_list<int> list) {
    for (int i : list) {
        set(i);
    }
}

value_set value_set::all() {
    return std::bitset<10>(0b1111111110);
}

value_set value_set::none() {
    return std::bitset<10>(0);
}

// bool value_set::contains(const std::bitset<10> &rhs) const {
//     return (*this & rhs) == rhs;
// }

value_set value_set::from_uint(uint32_t v) {
    return std::bitset<10>(v);
}

std::string value_set::to_string() const {
    std::ostringstream oss;
    bool first = true;
    for (size_t i = 0; i < 10; ++i) {
        if (at(i)) {
            if (!first) {
                oss << '/';
            }
            oss << i;
            first = false;
        }
    }
    return oss.str();
}

size_t value_set::count() const {
    return values.count();
}

int value_set::first() const {
    return std::bit_width(values.to_ulong()) - 1;
}

bool value_set::empty() const {
    return values.none();
}

bool value_set::operator!=(const value_set &rhs) const {
    return values != rhs.values;
}

value_set value_set::operator|(const value_set &rhs) const {
    return values | rhs.values;
}

value_set value_set::operator&(const value_set &rhs) const {
    return values & rhs.values;
}

value_set value_set::operator-(const value_set &rhs) const {
    return values & ~rhs.values;
}

value_set value_set::operator^(const value_set &rhs) const {
    return values ^ rhs.values;
}

value_set value_set::operator~() const {
    return ~values;
}

value_set & value_set::operator|=(const value_set &rhs) {
    values |= rhs.values;
    return *this;
}

value_set & value_set::operator&=(const value_set &rhs) {
    values &= rhs.values;
    return *this;
}

value_set::operator bool() const {
    return values.any();
}

void value_set::set(int i) {
    values.set(i);
}

void value_set::set(int i, bool value) {
    values.set(i, value);
}

bool value_set::at(int i) const {
    return values.test(i);
}

bool value_set::operator==(const value_set &rhs) const {
    return values == rhs.values;
}


