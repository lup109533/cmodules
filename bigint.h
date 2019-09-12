#ifndef __CMODULES_BIGINT__
#define __CMODULES_BIGINT__

#include "types.h"
#include "expr.h"

/** Type for integer values of any size **/
typedef struct {
    s64*   block;
    s64*   last;
    size_t len;
} __cmodules_bigint_struct;
typedef __cmodules_bigint_struct* bigint;

#define __cmodules_bigint_new_macro(S, ...) __cmodules_bigint_new(S, __VA_NARGS__(__VA_ARGS__), ## __VA_ARGS__)
bigint __cmodules_bigint_new(size_t, size_t, ...);
void del_bigint(bigint);

bigint add_bigint(bigint, bigint);
bigint sub_bigint(bigint, bigint);
bigint inv_bigint(bigint);
bigint mul_bigint(bigint, bigint);
bigint div_bigint(bigint, bigint);
bigint pow_bigint(bigint, bigint);
bigint add_bigint(bigint, bigint);

#include "src/bigint.c"

#endif // __CMODULES_BIGINT__
