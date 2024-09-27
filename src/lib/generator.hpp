//
// Created by Obi Davis on 10/09/2024.
//

#ifndef COMPLETE_BOARD_HPP
#define COMPLETE_BOARD_HPP

#include <random>
#include <string>
#include <mutex>

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

class puzzle_generator {
public:
    explicit puzzle_generator(unsigned int seed) : gen(seed) {}
    std::string generate_puzzle(std::string_view solution);
    std::string generate_solution();
private:
    std::mt19937 gen;
};

#endif //COMPLETE_BOARD_HPP
