//
// Created by Obi Davis on 14/10/2024.
//

#include "generate.hpp"
#include "enum.hpp"

#include <algorithm>
#include <mutex>
#include <vector>
#include <optional>
#include <memory>
#include <future>

#include "magic_enum.hpp"
#include "CLI/CLI.hpp"

#include "grader.hpp"


class thread_safe_seed_generator {
public:
    explicit thread_safe_seed_generator(unsigned int seed) : gen(seed) {}
    unsigned int operator()() {
        std::lock_guard lock(mutex);
        std::uniform_int_distribution<unsigned int> dist;
        return dist(gen);
    }
private:
    std::mt19937 gen;
    std::mutex mutex;
};

static std::optional<generation_result> try_generate_one(
        unsigned int seed,
        const std::vector<strategy_fn> &allowed_strategies,
        const std::vector<strategy_fn> &required_strategies,
        float min_difficulty,
        float max_difficulty);

void setup_generate(CLI::App &app) {
    auto opt = std::make_shared<generation_options>();
    auto generate = app.add_subcommand("generate", "Generate sudoku puzzles");

    generate->add_option("-c, --count", opt->count, "Number of puzzles to generate")
        ->option_text(" ")
        ->default_val(1)
        ->check(CLI::PositiveNumber);

    generate->add_option("-d, --difficulty", opt->difficulty_range, "Difficulty range")
        ->option_text("[min, max (optional)]")
        ->default_val(std::vector{0.0f, std::numeric_limits<float>::infinity()})
        ->expected(1, 2);

    generate->add_option("-s, --seed", opt->seed, "Random seed")
        ->option_text(" ")
        ->default_val(std::random_device{}());

    generate->add_option("-j, --threads", opt->threads, "Number of threads")
        ->option_text(" ")
        ->default_val(1)
        ->check(CLI::PositiveNumber);

    generate->add_option("-f, --format", opt->format.format_type, "Output format")
        ->option_text("[plain, json]")
        ->transform(CLI::CheckedTransformer(enum_name_map<format_options::type>()))
        ->default_val("plain");

    generate->add_option("-i, --indent", opt->format.indent, "Indentation level for JSON output (-1 for unformatted)")
        ->option_text(" ")
        ->check(CLI::Range(-1, 8))
        ->default_val(2);

    generate->add_option("-a, --allowed-strategies", opt->allowed_strategies, "Allowed strategies")
        ->option_text([] {
            std::ostringstream oss;
            oss << "[";
            for (const auto &name : magic_enum::enum_names<strategy_type>()) {
                oss << name << ", ";
            }
            return oss.str().substr(0, oss.str().size() - 2) + "]";
        }())
        ->transform(CLI::CheckedTransformer(enum_name_map<strategy_type>()))
        ->default_val(all_strategy_types);

    generate->add_option("-r, --required-strategies", opt->required_strategies, "Required strategies")
        ->option_text("[Same as allowed strategies]")
        ->transform(CLI::CheckedTransformer(enum_name_map<strategy_type>()));

    generate->callback([opt]() {
        auto results = run_generate(*opt);
        format(results, opt->format);
    });
}

std::vector<generation_result> run_generate(const generation_options &opt) {
    thread_safe_seed_generator gen(opt.seed);

    std::vector<strategy_fn> allowed_strategies;
    std::ranges::transform(opt.allowed_strategies, std::back_inserter(allowed_strategies), [](strategy_type type) {
        return strategy_map[static_cast<size_t>(type)];
    });

    std::vector<strategy_fn> required_strategies;
    std::ranges::transform(opt.required_strategies, std::back_inserter(required_strategies), [](strategy_type type) {
        return strategy_map[static_cast<size_t>(type)];
    });

    auto generate_task = [&](unsigned int count) {
        std::vector<generation_result> results;
        while (results.size() < count) {
            unsigned int seed = gen();
            auto result = try_generate_one(
                seed,
                allowed_strategies,
                required_strategies,
                opt.difficulty_range[0],
                opt.difficulty_range[1]);
            if (result) {
                results.push_back(std::move(*result));
            }
        }
        return results;
    };

    std::vector<std::future<std::vector<generation_result>>> futures;
    unsigned int puzzles_per_thread = opt.count / opt.threads;
    unsigned int remaining_puzzles = opt.count % opt.threads;
    for (int i = 1; i < opt.threads; ++i) {
        futures.push_back(std::async(std::launch::async, generate_task, puzzles_per_thread));
    }

    std::vector results = generate_task(puzzles_per_thread + remaining_puzzles);
    for (auto &f : futures) {
        std::vector thread_results = f.get();
        results.insert(
            results.end(),
            std::make_move_iterator(thread_results.begin()),
            std::make_move_iterator(thread_results.end())
            );
    }

    return results;
}


std::optional<generation_result> try_generate_one(
        unsigned int seed,
        const std::vector<strategy_fn> &allowed_strategies,
        const std::vector<strategy_fn> &required_strategies,
        float min_difficulty,
        float max_difficulty) {
    // std::vector<strategy_fn> allowed_strategies;
    // std::ranges::transform(options.allowed_strategies, std::back_inserter(allowed_strategies), []( strategy_type type) {
    //     return strategy_map[static_cast<size_t>(type)];
    // });
    //
    puzzle_generator generator(seed);
    std::string solution = generator.generate_solution();
    std::string puzzle = generator.generate_puzzle(solution);
    std::vector solve_path = solve(board{puzzle}, allowed_strategies);
    bool has_required_strategies =
        std::ranges::all_of(required_strategies, [&](strategy_fn strategy) {
            return std::ranges::any_of(solve_path, [&](const solution_step &step) {
                return std::ranges::any_of(step.eliminations, [&](const std::unique_ptr<base_elimination> &elim) {
                    return strategy_map[static_cast<size_t>(elim->type())] == strategy;
                });
            });
        });

    if (has_required_strategies) {
        float difficulty = grade(solve_path).grade();
        bool has_required_difficulty = difficulty >= min_difficulty && difficulty <= max_difficulty;
        if (has_required_difficulty) {
            return {{
                std::move(puzzle),
                std::move(solution),
                difficulty
            }};
        }
    }

    return std::nullopt;
}

