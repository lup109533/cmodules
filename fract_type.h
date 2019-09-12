#ifndef __CMODULES_FRACTTYPE__
#define __CMODULES_FRACTTYPE__

/** Defines fractional type to represent fractional values with infinite precision. **/

#include "types.h"

typedef struct {
    s32 num;
    s32 den;
} fract32;

typedef struct {
    s64 num;
    s64 den;
} fract64;

#define fract32_const(N) ((fract32){N,1})
#define fract64_const(N) ((fract64){N,1})

#define fract_solve(T, X) ((T) X.num / (T) X.den)

#define fract_add(X, Y) _Generic((X),\
                                 fract32: __cmodules_fract32_add,\
                                 fract64: __cmodules_fract64_add\
                                )(X, Y)

fract32  __cmodules_fract32_add(fract32,  fract32);
fract64  __cmodules_fract64_add(fract64,  fract64);

#define fract_inv(X) _Generic(\
                         (X),\
                         fract32: __cmodules_fract32_inv,\
                         fract64: __cmodules_fract64_inv\
                     )(X)

fract32  __cmodules_fract32_inv(fract32);
fract64  __cmodules_fract64_inv(fract64);

#define fract_mul(X, Y) _Generic(\
                                 (X),\
                                 fract32: __cmodules_fract32_mul,\
                                 fract64: __cmodules_fract64_mul\
                                )(X, Y)

fract32  __cmodules_fract32_mul(fract32,  fract32);
fract64  __cmodules_fract64_mul(fract64,  fract64);

#define fract_div(X, Y) (fract_mul(X, fract_inv(Y)))

#define fract_map(X, f) ((typeof(X)) {f(X.num), f(X.den)})

#include "src/fract_type.c"

#endif // __CMODULES_FRACTTYPE__
