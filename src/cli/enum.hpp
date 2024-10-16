//
// Created by Obi Davis on 16/10/2024.
//

#ifndef ENUM_HPP
#define ENUM_HPP

#include "magic_enum.hpp"

template <typename T>
auto enum_name_map() {
    static constexpr auto swap_pair = [](auto &&pair) {
        return std::make_pair(pair.second, pair.first);
    };
    static constexpr auto swap_pairs =
        []<typename A, typename B, std::size_t ...Is>(const std::array<std::pair<A, B>, sizeof...(Is)> &arr, std::index_sequence<Is...>) {
            return std::array{swap_pair(arr[Is])...};
        };

    return swap_pairs(magic_enum::enum_entries<T>(), std::make_index_sequence<magic_enum::enum_count<T>()>{});
}

#endif //ENUM_HPP
