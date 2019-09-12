#ifndef __CMODULES_MATH__
#define __CMODULES_MATH__

#include "types.h"
#include "macro.h"
#include "error.h"

#define __CMODULES_NOP(x) x

#define math(T) math_ ## T

#define __cmodules_make_math_lib(T) \
struct __cmodules_math_ ## T ## _library { \
	T (*abs) (T); \
	T (
};

#endif