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

typedef long     pointer;

typedef wchar_t  Char;
typedef char     c_char;

typedef union {
	bool    _bool;
	
	s8      _s8;
	s16     _s16;
	s32     _s32;
	s64     _s64;
	
	u8      _u8;
	u16     _u16;
	u32     _u32;
	u64     _u64;
	
	f32     _f32;
	f64     _f64;
	
	pointer _pointer;
	
	Char    _Char;
	c_char  _c_char;
} Any;

// Singly-linked list
struct __cmodules_list_single_node {
	Any val;
	struct __cmodules_list_single_node* next;
};
typedef struct __cmodules_list_single_node* ListNode;

struct __cmodules_list {
	ListNode head;
	size_t   len;
	size_t   mem;
};
typedef struct __cmodules_list* List;

// Doubly-linked list
struct __cmodules_list_double_node {
	Any val;
	struct __cmodules_list_double_node* next;
	struct __cmodules_list_double_node* prev;
};
typedef struct __cmodules_list_double_node* DoubleListNode;

struct __cmodules_double_list {
	DoubleListNode head;
	size_t         len;
	size_t         mem;
};
typedef struct __cmodules_double_list* DoubleList;

// Dictionary
struct __cmodules_dict_node {
	Any   val;
	Char* key;
	bool  valid;
	bool  collision;
	struct __cmodules_dict_node* next;
};
typedef struct __cmodules_dict_node* DictNode;

struct __cmodules_dict {
	DictNode* val;
	size_t    len;
	size_t    mem;
};
typedef struct __cmodules_dict* Dict;

#endif
