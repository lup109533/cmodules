#ifndef __CMODULES_STRING__
#define __CMODULES_STRING__

#include "types.h"
#include "macro.h"
#include "error.h"

struct __cmodules_string {
	Char* val;
	size_t len;
};
typedef struct __cmodules_string* String;

inline String new(const Char*);
inline Error  del(String);

struct __cmodules_string_library {
	String (*new)(const Char*);
	Error  (*del)(String);
};

extern struct __cmodules_string_library string;

#include "src/string.c"

#endif
