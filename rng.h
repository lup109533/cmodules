#ifndef __CMODULES_RANDOM__
#define __CMODULES_RANDOM__

#include "types.h"
#include "macro.h"
#include "error.h"
#include "ext/splitmix64.c"
#include "ext/xoroshiro128plus.c"

void __cmodules_rng_seed(u64);
u64  __cmodules_rng_next();
u64  __cmodules_rng_integer(u64,u64);
f64  __cmodules_rng_real();
f64  __cmodules_rng_uniform(f64,f64);
f64  __cmodules_rng_normal(f64,f64);

struct __cmodules_rng_library {
	void (*seed)    (u64);
	u64  (*next)    ();
	u64  (*integer) (u64,u64);
	f64  (*real)    ();
	f64  (*uniform) (f64,f64);
	f64  (*normal)  (f64,f64);
};

extern struct __cmodules_rng_library rng;

#include "src/rng.c"

#endif
