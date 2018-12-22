#ifndef __CMODULES_TYPES__
#define __CMODULES_TYPES__

/*** Standard cmodules types ***/

#include <stdint.h>
#include <stdbool.h> // Include bool types and true/false macros
#include <wchar.h>   // Include widechar as standard char

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

typedef wchar_t  Char;

typedef char     c_char;
typedef char*    c_string;

#endif
