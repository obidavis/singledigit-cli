//
// Created by Obi Davis on 30/08/2024.
//

#ifndef ELIMINATION_HPP
#define ELIMINATION_HPP

#include "../board/board.hpp"
#include "../board/value_set.hpp"
#include "../board/cell_set.hpp"
#include "../board/constraint_set.hpp"
#include <variant>
#include <string>


struct basic_elimination {
    cell_index set_cell;
    int eliminated_value;
    cell_set eliminated_cells;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const;
    void apply(board &b) const;
    [[nodiscard]] bool operator==(const basic_elimination &other) const = default;
};

struct value_combination_elimination {
    cell_set cells;
    value_set values;
    constraint_set c_set;
    [[nodiscard]] std::string to_string() const;
    void apply(board &b) const;
    [[nodiscard]] bool operator==(const value_combination_elimination &other) const = default;
};

struct cell_combination_elimination {
    cell_set cells;
    value_set values;
    constraint_set c_set;
    cell_set eliminated_cells;
    [[nodiscard]] std::string to_string() const;
    void apply(board &b) const;
    [[nodiscard]] bool operator==(const cell_combination_elimination &other) const = default;
};

struct intersection_removal_elimination {
    cell_set intersection;
    cell_set eliminated_cells;
    value_set eliminated_values;
    std::pair<constraint_set, constraint_set> c_sets;
    [[nodiscard]] std::string to_string() const;
    void apply(board &b) const;
    [[nodiscard]] bool operator==(const intersection_removal_elimination &other) const = default;
};

using elimination_variant = std::variant<
    basic_elimination,
    value_combination_elimination,
    cell_combination_elimination,
    intersection_removal_elimination
>;

template <typename T>
concept Elimination = requires (T t, elimination_variant variant, board &b) {
    { std::get<T>(variant) } -> std::same_as<T&>;
    { t.to_string() } -> std::same_as<std::string>;
    { t.apply(b) };
};

struct elimination {
    template <Elimination E>
    requires (!std::same_as<std::remove_cvref_t<E>, elimination>) // Hack because elimination actually satisfies Elimination?
    explicit elimination(E &&variant) : variant(std::forward<E>(variant)) {}
    elimination(const elimination &other) = default;
    elimination(elimination &&other) noexcept = default;
    elimination &operator=(const elimination &other) = default;
    elimination &operator=(elimination &&other) noexcept = default;
    ~elimination() = default;

    [[nodiscard]] std::string to_string() const {
        return std::visit([](const auto &result) { return result.to_string(); }, variant);
    }

    void apply(board &b) const {
        std::visit([&b](const auto &result) { result.apply(b); }, variant);
    }

    template <Elimination E>
    const E &get() const {
        return std::get<E>(variant);
    }

    template <Elimination E>
    E &get() {
        return std::get<E>(variant);
    }
private:
    elimination_variant variant;
};

#endif //ELIMINATION_HPP
