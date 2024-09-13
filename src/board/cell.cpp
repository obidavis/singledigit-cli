#include "cell.hpp"

#include <cassert>

cell::cell(cell_index index) : _index(index), _candidates(value_set::all()), _solved(false) {}

cell::cell(cell_index index, int value) : _index(index), _candidates({value}), _solved(true) {
    assert(value > 0 && value < 10);
}

cell::cell(cell_index index, value_set candidates) : _index(index), _candidates(candidates), _solved(false) {}

int cell::value() const {
    assert(is_solved());
    return _candidates.first();
}

value_set cell::candidates() const {
    assert(!is_solved());
    return _candidates;
}

bool cell::is_solved() const {
    return _solved;
}

std::string cell::to_string() const {
    return _candidates.to_string();
}

cell_index cell::index() const {
    return _index;
}

void cell::solve(int value) {
    assert(value > 0 && value < 10);
    _candidates = {value};
    _solved = true;
}

void cell::remove_candidate(int value) {
    assert(!is_solved());
    _candidates.set(value, false);
}

void cell::remove_candidates(value_set values) {
    assert(!is_solved());
    _candidates &= ~values;
}


