#pragma once

#if !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)
#error L header file included directly, Include l/l.hpp. 
#endif // !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)

#define UNUSED(x) {(void) (x);}

#define ATTR(x) __attribute__((x))

#define L_PACKED ATTR(packed)
#define L_HOT ATTR(hot)
#define L_COLD ATTR(cold)
#define L_FORCE_OPTIMZIE_LEVEL(LEVEL) ATTR(optimize(LEVEL))
#define L_ALIGN(x) ATTR(aligned(x))
#define L_NORETURN ATTR(noreturn)
#define L_PURE ATTR(pure) ATTR(const)
#define L_PRIVATE ATTR(visibility("hidden"))
#define L_USED ATTR(used)