#include <stdlib.h>
#include <tgmath.h>

struct __cmodules_rng_library rng = {
	.seed    = &__cmodules_rng_seed,
	.next    = &__cmodules_rng_next,
	.integer = &__cmodules_rng_integer,
	.real    = &__cmodules_rng_real,
	.uniform = &__cmodules_rng_uniform,
	.normal  = &__cmodules_rng_normal,
};

/***************************/
/* Init seed for generator */
/***************************/
void __cmodules_rng_seed(u64 seed) {
	__splitmix64_seed = seed;

	__xoroshiro128plus_state[0] = __splitmix64_next();
	__xoroshiro128plus_state[1] = __splitmix64_next();
}

/********************************************/
/* Get next pseudo-random value in sequence */
/********************************************/
u64 __cmodules_rng_next() {
	return __xoroshiro128plus_next();
}

/**************************************/
/* Get random integer between a and b */
/**************************************/
u64 __cmodules_rng_integer(u64 a, u64 b) {
#ifdef __CMODULES_ERROR_CHECK
	if (a > b) {
		u64 tmp = a;
		a = b;
		b = tmp;
	}
#endif
	return (__xoroshiro128plus_next() % (b - a + 1)) + a;
}

/*******************************************************/
/* Get next pseudo-random value in sequence as a float */
/*******************************************************/
f64 __cmodules_rng_real() {
	u64 intermediate = __xoroshiro128plus_next();
	return *((f64*)(&intermediate));
}

/***************************************************************************/
/* Get a real value between a and b with uniformly distributed probability */
/***************************************************************************/
f64 __cmodules_rng_uniform(f64 a, f64 b) {
#ifdef __CMODULES_ERROR_CHECK
	if (a > b) {
		f64 tmp = a;
		a = b;
		b = tmp;
	}
#endif
	f64 step = (b - a) / ((f64) ((u64) -1));
	return a + ((f64) __xoroshiro128plus_next())*step;
}

/*********************************************************************************************/
/* Get a real value according to normal distribution of mean mu and standard deviation sigma */
/*********************************************************************************************/
f64 __cmodules_rng_normal(f64 mu, f64 sigma) {
    return 1.0;
}
