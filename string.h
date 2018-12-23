#ifndef __CMODULES_STRING__
#define __CMODULES_STRING__

#include "types.h"
#include "macro.h"
#include "error.h"

struct __cmodules_string {
	Char*  val;
	size_t len;
	size_t mem;
};
typedef struct __cmodules_string* String;

String __cmodules_string_new      (const Char*);
Error  __cmodules_string_del      (String);
Error  __cmodules_string_set      (String, const Char*);
String __cmodules_string_alloc    (size_t);
Error  __cmodules_string_resize   (String, size_t);
Error  __cmodules_string_to_upper (String);
Error  __cmodules_string_to_lower (String);
Error  __cmodules_string_strip    (String);

struct __cmodules_string_library {
	String (*new)      (const Char*);
	Error  (*del)      (String);
	Error  (*set)      (String, const Char*);
	String (*alloc)    (size_t);
	Error  (*resize)   (String, size_t);
	Error  (*to_lower) (String);
	Error  (*to_upper) (String);
	Error  (*strip)    (String);
};

extern struct __cmodules_string_library string;

#include "src/string.c"

#endif
