#include <algorithm>
#include <ranges>

#include <fmt/core.h>

#include "fish.hpp"

#include "board/constraint_set.hpp"
#include "board/value_set.hpp"

template <size_t N>
static void fish_impl(const std::array<constraint_set, N> &constraint_sets1, const std::array<constraint_set, N> &constraint_sets2, const board &bd, std::vector<std::unique_ptr<base_elimination>> &eliminations) {
    cell_set open_cells = bd.open_cells();
    cell_set cs1 = open_cells & [constraint_sets1]<size_t ...Is>(std::index_sequence<Is...>) {
        return (constraint_sets1[Is] | ...);
    }(std::make_index_sequence<N>());
    cell_set cs2 = open_cells & [constraint_sets2]<size_t ...Is>(std::index_sequence<Is...>) {
        return (constraint_sets2[Is] | ...);
    }(std::make_index_sequence<N>());

    cell_set overlap = cs1 & cs2;
    cell_set cs1_without_overlap = cs1 - overlap;
    cell_set cs2_without_overlap = cs2 - overlap;

    value_set values_in_overlap = bd[overlap].open_values();
    for (size_t i = 0; i < N; ++i) {
        value_set cs1_open_values = bd[cs2 & constraint_sets1[i]].open_values();
        value_set cs2_open_values = bd[cs1 & constraint_sets2[i]].open_values();
        values_in_overlap &= cs1_open_values & cs2_open_values;
    }

    for (int value : values_in_overlap) {
        cell_set cells_in_cs1_without_overlap_containing_value = bd[cs1_without_overlap].containing(value);
        cell_set cells_in_cs2_without_overlap_containing_value = bd[cs2_without_overlap].containing(value);

        if (cells_in_cs1_without_overlap_containing_value.empty() &&
           !cells_in_cs2_without_overlap_containing_value.empty()) {
            auto elimination = std::make_unique<fish_elimination<N>>(
                value_set{value}, cells_in_cs2_without_overlap_containing_value, std::array<cell_set, N>{cs1, cs2});
            eliminations.push_back(std::move(elimination));
        }

        if (cells_in_cs2_without_overlap_containing_value.empty() &&
           !cells_in_cs1_without_overlap_containing_value.empty()) {
            auto elimination = std::make_unique<fish_elimination<N>>(
                value_set{value}, cells_in_cs1_without_overlap_containing_value, std::array<cell_set, N>{cs1, cs2});
            eliminations.push_back(std::move(elimination));
        }
    }
}


// Main function to generate all combinations of length N from 0 to 9
template <size_t N>
static auto generate_combinations() {
    std::array indices = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::array<bool, 9> mask{};
    std::fill_n(mask.begin(), N, true);

    std::vector<std::array<int, N>> combinations;
    do {
        std::array<int, N> combination{};
        std::copy_if(indices.begin(), indices.end(), combination.begin(), [&mask](int i) { return mask[i]; });
        combinations.push_back(combination);
    } while (std::prev_permutation(mask.begin(), mask.end()));
    return combinations;
}


template <size_t N>
static std::vector<std::unique_ptr<base_elimination>> fish(const board &bd) {
    std::vector<std::unique_ptr<base_elimination>> eliminations;
    static constexpr auto make_constraint_set_array = [](const std::array<int, N> &indices, constraint_set_type type) {
        return [&indices, type]<size_t ...Is>(std::index_sequence<Is...>) {
            return std::array{constraint_set{type, indices[Is]}...};
        }(std::make_index_sequence<N>());
    };
    auto combinations = generate_combinations<N>();
    for (const auto &combination1 : combinations) {
        for (const auto &combination2 : combinations) {
            auto rows = make_constraint_set_array(combination1, constraint_set_type::row);
            auto cols = make_constraint_set_array(combination2, constraint_set_type::column);
            fish_impl(rows, cols, bd, eliminations);
        }
    }
    return eliminations;
}

std::vector<std::unique_ptr<base_elimination>> x_wing(const board &bd) {
    return fish<2>(bd);
}

std::vector<std::unique_ptr<base_elimination> > swordfish(const board &bd) {
    return fish<3>(bd);
}

template struct fish_elimination<2>;
template struct fish_elimination<3>;
// template struct fish_elimination<4>;

template <>
std::string fish_elimination<2>::to_string() const {
    return fmt::format("X-Wing: {} in {} and {} in {}",
                       eliminated_values.to_string(), sets[0].to_string(), eliminated_values.to_string(), sets[1].to_string());
}

template <>
std::string fish_elimination<2>::name() const {
    return "X-Wing";
}

template <>
std::string fish_elimination<3>::to_string() const {
    return fmt::format("Swordfish: {} in {} and {} in {}",
                       eliminated_values.to_string(), sets[0].to_string(), eliminated_values.to_string(), sets[1].to_string());
}

template <>
std::string fish_elimination<3>::name() const {
    return "Swordfish";
}





