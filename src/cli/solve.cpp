//
// Created by Obi Davis on 16/10/2024.
//

#include "solve.hpp"

#include <string>
#include <memory>

#include "enum.hpp"
#include "magic_enum.hpp"
#include "CLI/CLI.hpp"

#include "solver.hpp"


void setup_solve(CLI::App &app) {
    auto opt = std::make_shared<solve_options>();
    auto solve = app.add_subcommand("solve", "Solve a sudoku board");

    solve->add_option("board", opt->board, "The board to solve")
        ->option_text(" ")
        ->required();

    solve->add_flag("-S, --single-step", opt->single_step, "Solve one step at a time");

    solve->add_option("-f, --format", opt->format.format_type, "Output format")
        ->option_text("[plain, json]")
        ->transform(CLI::CheckedTransformer(enum_name_map<format_options::type>()))
        ->default_val(format_options::type::plain);

    solve->add_option("-i, --indent", opt->format.indent, "Indentation level for JSON output (-1 for unformatted)")
        ->option_text(" ")
        ->check(CLI::Range(-1, 8))
        ->default_val(2);

    solve->callback([opt]() {
        auto results = run_solve(*opt);
        format(results, opt->format);
    });
}

std::vector<solution_step> run_solve(const solve_options &opt) {
    board bd(opt.board);
    std::vector<solution_step> result;
    if (opt.single_step) {
        solution_step step = solve_step(bd, all_strategies);
        result.emplace_back(std::move(step));
    } else {
        result = solve(bd, all_strategies);
    }
    return result;
}

