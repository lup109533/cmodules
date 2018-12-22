#ifndef __CMODULES_ERROR___
#define __CMODULES_ERROR___

#define __CMODULES_ERROR_CHECK

enum __cmodules_error {
	NO_ERROR,
	MALLOC_FAILURE,
	NULL_ARGUMENT,
};

typedef enum __cmodules_error Error;

extern Error __cmodules_error_var;
#define cerror __cmodules_error_var

#include "src/error.c"

#endif
