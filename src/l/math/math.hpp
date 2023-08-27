#pragma once

#include <string>
#include <cstdio>

#include "../utils/utils.hpp"

#if !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)
#error L header file included directly, Include l/l.hpp. 
#endif // !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)

#if defined(STRICT) && defined(QUADMATH_USED)
#error Implement math functions for quadmath
#endif // STRICT

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

    // TODO: More optimize sin, cos, tan
    template <typename T>
    inline T sin(T a);
    template <typename T>
    inline T cos(T a);
    template <typename T>
    inline T tan(T a);
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
#endif // __SIZEOF_LONG_DOUBLE__ != __SIZEOF_DOUBLE__


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

template <typename T>
inline T LMath::sin(T a) {
    if (std::round(std::fmod(a, T(M_PI)) * 100000) / 100000 == T(3.14159))
        return T(0);
    return std::sin(a);
}

template <typename T>
inline T LMath::cos(T a) {
    if (std::round(std::fmod(a, T(M_PI)) * 100000) / 100000 == T(3.14159))
        return T(-1);
    return std::cos(a);
}

template <typename T>
inline T LMath::tan(T a) {
    if (std::round(std::fmod(a, T(M_PI)) * 100000) / 100000 == T(3.14159))
        return T(0);
    return std::tan(a);
}
