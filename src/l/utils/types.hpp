#pragma once

#if !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)
#error L header file included directly, Include l/l.hpp. 
#endif // !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)

#include <cinttypes>
#include <cstddef>
#include <cfloat>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#if defined(__SIZEOF_LONG_DOUBLE__)
    #if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_DOUBLE__
        #define BIGF f64
    #elif __SIZEOF_LONG_DOUBLE__ == 10
        typedef long double f80;
        #define BIGF f80
    #elif __SIZEOF_LONG_DOUBLE__ == 16
        typedef long double f128;
        #define F128_LD
        #define BIGF f128
    #else
        #define BIGF f64
    #endif
#endif

#if defined(QUADMATH_USED)
    #if !defined(F128_LD)
        extern "C" {
        #include <quadmath.h>
        }
        #undef BIGF
        #define BIGF __float128

        #error Handle using quadmath
    #endif
#endif

typedef BIGF bigf;
