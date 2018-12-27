#ifndef __CMODULES_STRING__
#define __CMODULES_STRING__

#include "types.h"
#include "macro.h"
#include "error.h"

/*** Wide character managed String objects; internal representation is platform-dependent       ***/
/*** (UTF-16 or UTF-32 in virtually all cases), the io library will convert this representation ***/
/*** to UTF-8.                                                                                  ***/

// String
struct __cmodules_string {
	Char*  val;
	size_t len;
	size_t mem;
};
typedef struct __cmodules_string* String;

String __cmodules_string_new      (const Char*);
Error  __cmodules_string_del      (String);
Error  __cmodules_string_set      (String, const Char*);
String __cmodules_string_copy     (String);
String __cmodules_string_slice    (String, size_t, size_t);
String __cmodules_string_alloc    (size_t);
Error  __cmodules_string_resize   (String, size_t);
Error  __cmodules_string_to_upper (String);
Error  __cmodules_string_to_lower (String);
Error  __cmodules_string_strip    (String);
Error  __cmodules_string_replace  (String, const Char*, const Char*);
size_t __cmodules_string_search   (String, const Char*, size_t);
bool   __cmodules_string_equals   (String, const Char*);
Error  __cmodules_string_concat   (String, const Char*);
Error  __cmodules_string_insert   (String, const Char*, size_t);
Error  __cmodules_string_fill     (String, const Char*);
Error  __cmodules_string_clear    (String);
Error  __cmodules_string_filter   (String, bool (*fun)(Char));
Error  __cmodules_string_map      (String, Char (*fun)(Char));

struct __cmodules_string_library {
	String (*new)      (const Char*);
	Error  (*del)      (String);
	Error  (*set)      (String, const Char*);
	String (*copy)     (String);
	String (*slice)    (String, size_t, size_t);
	String (*alloc)    (size_t);
	Error  (*resize)   (String, size_t);
	Error  (*to_lower) (String);
	Error  (*to_upper) (String);
	Error  (*strip)    (String);
	Error  (*replace)  (String, const Char*, const Char*);
	size_t (*search)   (String, const Char*, size_t);
	bool   (*equals)   (String, const Char*);
	Error  (*concat)   (String, const Char*);
	Error  (*insert)   (String, const Char*, size_t);
	Error  (*fill)     (String, const Char*);
	Error  (*clear)    (String);
	Error  (*filter)   (String, bool (*fun)(Char));
	Error  (*map)      (String, Char (*fun)(Char));
};

extern struct __cmodules_string_library string;

#include "src/string.c"

#endif
