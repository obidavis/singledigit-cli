//
// Created by Obi Davis on 16/10/2024.
//

#ifndef ELIMINATIONS_MATCHER_HPP
#define ELIMINATIONS_MATCHER_HPP

#include "gmock/gmock.h"

template <typename ...Ts>
auto UnorderedElementsAreConcrete(Ts && ...args) {
    using namespace ::testing;
    return UnorderedElementsAre(Pointee(WhenDynamicCastTo<const typename std::remove_reference<Ts>::type &>(args))...);
}


#endif //ELIMINATIONS_MATCHER_HPP
