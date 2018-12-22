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

String __cmodules_string_new   (const Char*);
Error  __cmodules_string_del   (String);
String __cmodules_string_alloc (size_t);

struct __cmodules_string_library {
	String (*new)   (const Char*);
	Error  (*del)   (String);
	String (*alloc) (size_t);
};

extern struct __cmodules_string_library string;

#include "src/string.c"

#endif
