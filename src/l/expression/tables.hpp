#pragma once

#define L_PRIVATE_INCLUDE
#include <unordered_map>
#include "../utils/utils.hpp"

#ifndef L_PRIVATE_INCLUDE
#error TABLES.HPP IS LIBRARIES PRIVATE FILE
#endif

static const std::unordered_map<char, i8> precedence = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'^', 3},
    {'c', 4},
    {'s', 4},
    {'t', 4}
};

template<typename T>
static const std::unordered_map<char, T(*)(T, T)> operator_functions {
    {'+',   &LMath::add<T>},
    {'-',   &LMath::sub<T>}, 
    {'*',   &LMath::mul<T>},
    {'/',   &LMath::div<T>},
    {'^',   &LMath::pow<T>},
};

template<typename T>
static const std::unordered_map<char, T(*)(T)> mathematical_function_functions {
    {'s', &LMath::sin<T>},
    {'c', &LMath::cos<T>},
    {'t', &LMath::tan<T>},
};

static const std::unordered_map<char, std::string> functions = {
    {'s', "sin"},
    {'c', "cos"},
    {'t', "tan"},
};

template<typename T>
static const std::unordered_map<std::string, T> constants = {
    {"pi", T(M_PI)},
    {"e", T(M_E)}
};