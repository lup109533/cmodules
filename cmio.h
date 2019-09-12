#ifndef __CMODULES_CMIO__
#define __CMODULES_CMIO__
/*
 *
 */

#include "types.h"

/*** Input & string parsing ***/
#define parse(T, str) __cmodules_parse_ ## T(str)
s8  __cmodules_parse_s8 (string);
s16 __cmodules_parse_s16(string);
s32 __cmodules_parse_s32(string);
s64 __cmodules_parse_s64(string);

u8  __cmodules_parse_u8 (string);
u16 __cmodules_parse_u16(string);
u32 __cmodules_parse_u32(string);
u64 __cmodules_parse_u64(string);

f32 __cmodules_parse_f32(string);
f64 __cmodules_parse_f64(string);

complex __cmodules_parse_complex(string);

raw_string __cmodules_parse_raw_string(string);
string     __cmodules_parse_string    (string);
array      __cmodules_parse_array     (string);
list       __cmodules_parse_list      (string);
dict       __cmodules_parse_dict      (string);

raw_string __cmodules_infer_type(string);

#include "src/cmio.c"

#endif // __CMODULES_CMIO__
