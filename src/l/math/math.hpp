#pragma once

#include <string>

#include "../utils/utils.hpp"

#ifdef STRICT
static_assert(false, "Implement math function detection for each floating point type")
#endif

namespace LMath
{
    template <typename T>
    inline T floating_from_string(std::string s);
    template <typename T>
    inline T add(T a, T b);
    template <typename T>
    inline T sub(T a, T b);
    template <typename T>
    inline T mul(T a, T b);
    template <typename T>
    inline T div(T a, T b);
    template <typename T>
    inline T pow(T a, T b);
}

template <>
inline f32 LMath::floating_from_string(std::string s) {
    return std::stof(s);
}   

template <>
inline f64 LMath::floating_from_string(std::string s) {
    return std::stod(s);
}

#if __SIZEOF_LONG_DOUBLE__ != __SIZEOF_DOUBLE__
template <>
inline bigf LMath::floating_from_string(std::string s) {
    return std::stold(s);
}
#endif


template <typename T>
inline T LMath::add(T a, T b) {
    return a + b;
}

template <typename T>
inline T LMath::sub(T a, T b) {
    return a - b;
}

template <typename T>
inline T LMath::mul(T a, T b) {
    return a * b;
}

template <typename T>
inline T LMath::div(T a, T b) {
    return a / b;
}

// TODO: Implement a fast pow
template <typename T>
inline T LMath::pow(T a, T b) {
    T result = a;
    
    for (int i = 0; i < (b-1); i++)
        result *= a;
        
    return result;
}