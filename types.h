#ifndef __CMODULES_TYPES__
#define __CMODULES_TYPES__

/*** Standard cmodules types ***/

#include "macro.h"
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>   // Include widechar as standard char
#include <string.h>  // For raw_string manipulation
#include <complex.h>

/** Fundamental types **/
typedef _Bool bool;
#define true  ((_Bool) !0)
#define false ((_Bool)  0)

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

typedef double _Complex cf64;

typedef u8       byte;
typedef char*    raw_string;
typedef u32      uchar;
typedef void*    raw_pointer;

/** Comparison struct **/
typedef struct {
    bool eq : 1;
    bool ne : 1;
    bool gt : 1;
    bool ge : 1;
    bool lt : 1;
    bool le : 1;
} cmp_result;

/** Special function types **/
typedef raw_pointer (*copy_function)(raw_pointer);
typedef void        (*del_function)(raw_pointer);
typedef size_t      (*hash_function)(raw_pointer, size_t);
typedef cmp_result  (*cmp_function)(raw_pointer, raw_pointer);

/** Container types **/
// string
// Stores characters internally as 21-bit UTF-8 code points;
// just-in-time conversion to UTF-8 is performed for printing.
typedef struct {
    uchar* val;
    size_t len; // Length of the used portion of the array
    size_t mem; // Effective space occupied in memory, in bytes (used to check for reallocation)
} __cmodules_string_struct;
typedef __cmodules_string_struct* string;


// arrays
// Length-aware array types for integers of sizes 8, 16, 32, and 64, floats and pointers.
#include "src/types_array.c"
typedef struct {
    byte*         val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_array_struct;
typedef __cmodules_array_struct* array;

// list, stack
// Singly-linked lists that allow nodes with values of any type.
// list elements can be arbitrarily addressed.
// stack elements follow a FILO structure.
typedef struct __cmodules_list_node_struct{
    raw_pointer   val;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
    struct __cmodules_list_node_struct* next;
} __cmodules_list_node_struct;
typedef __cmodules_list_node_struct* list_node;

typedef struct {
    list_node head;
    size_t    len;
} __cmodules_list_struct;
typedef __cmodules_list_struct* list;

typedef struct {
    list_node head;
    size_t    len;
} __cmodules_stack_struct;
typedef __cmodules_stack_struct* stack;


// fifo
// Doubly-linked FIFO list
typedef struct __cmodules_fifo_node_struct{
    raw_pointer   val;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
    struct __cmodules_fifo_node_struct* prev;
    struct __cmodules_fifo_node_struct* next;
} __cmodules_fifo_node_struct;
typedef __cmodules_fifo_node_struct* fifo_node;

typedef struct {
    fifo_node head;
    fifo_node tail;
    size_t    len;
} __cmodules_fifo_struct;
typedef __cmodules_fifo_struct* fifo;

// pop(X)
// Pops head/tail element of stack/fifo object X and returns its value.
#define pop(X) _Generic(\
                        (X),\
                        stack: __cmodules_stack_pop,\
                         fifo: __cmodules_fifo_pop\
                       )(X)

raw_pointer __cmodules_stack_pop(stack);
raw_pointer __cmodules_fifo_pop (fifo);

// pop_node(X)
// Pops head/tail element of stack/fifo object X and returns it.
#define pop_node(X) _Generic(\
                        (X),\
                        stack: __cmodules_stack_pop_node,\
                         fifo: __cmodules_fifo_pop_node\
                    )

list_node __cmodules_stack_pop_node(stack);
fifo_node __cmodules_fifo_pop_node (fifo);

// dict
// key-to-value associative array that allows nodes of any type.
typedef struct __cmodules_dict_node_struct {
    raw_pointer  val;
    raw_pointer  key;
    bool   has_collision;
    struct __cmodules_dict_node_struct* next_for_key;
} __cmodules_dict_node_struct;
typedef __cmodules_dict_node_struct* dict_node;

typedef struct {
    dict_node*    vals;
    raw_pointer*  keys;
    size_t        len;
    size_t        bucket_size;      // Length of preallocated space
    raw_string    key_type_id;
    size_t        key_type_size;
    raw_string    val_type_id;
    size_t        val_type_size;
    copy_function key_type_copy;
    del_function  key_type_del;
    cmp_function  key_type_cmp;
    copy_function val_type_copy;
    del_function  val_type_del;
    cmp_function  val_type_cmp;
    hash_function hash_key;         // Function used to hash keys
} __cmodules_dict_struct;
typedef __cmodules_dict_struct* dict;

dict_node __cmodules_dict_get_node(dict, raw_pointer); // Special function to deal with collisions
bool      __cmodules_dict_has_key (dict, raw_pointer); // Special function to scan for keys
#define has_key __cmodules_dict_has_key

typedef struct {
    raw_pointer key;
    raw_pointer val;
    size_t pos;
} dict_pair;

// set
// Singly-linked list of values of any type, without repetition
typedef struct {
    list_node head;
    size_t    len;
} __cmodules_set_struct;
typedef __cmodules_set_struct* set;

bool __cmodules_set_has_node(set, list_node); // Special function to scan for existing values

/** Language extension macros **/
// len(X)
// Length of container object X.
#define len(X)      (X->len)

// is_empty(X)
// Returns true if container object X has length 0, false otherwise.
#define is_empty(X) (X->len == 0)

// new(T, ...)
// Initializer for container object of type T, value(s) for initialization are optional.
#define new(T, ...) __cmodules_ ## T ## _new_macro(__VA_ARGS__)
#define __cmodules_raw_string_new_macro(S) (strcpy(calloc(sizeof(char), sizeof(S)+1), S))
#define __cmodules_string_new_macro(...) __cmodules_string_new(1 + __VA_NPARGS__(__VA_ARGS__), ## __VA_ARGS__)
string __cmodules_string_new (size_t, ...);
#define __cmodules_array_new_macro(T, ...) __cmodules_array_new(#T, sizeof(T), sizeof((T[]){__VA_ARGS__})/sizeof(T), sizeof((T[]){__VA_ARGS__}), (raw_pointer)((T[]){__VA_ARGS__}))
array  __cmodules_array_new  (const raw_string, size_t, size_t, size_t, raw_pointer);
#define __cmodules_list_new_macro __cmodules_list_new
list   __cmodules_list_new   ();
#define __cmodules_fifo_new_macro __cmodules_fifo_new
fifo   __cmodules_fifo_new   ();
#define __cmodules_stack_new_macro __cmodules_stack_new
stack  __cmodules_stack_new  ();
#define __cmodules_dict_new_macro(T1, T2, ...) __cmodules_dict_new(#T1, sizeof(T1), #T2, sizeof(T2))
dict   __cmodules_dict_new   (const raw_string, size_t, const raw_string, size_t);
#define __cmodules_set_new_macro __cmodules_set_new
set    __cmodules_set_new    ();

// del(X)
// Safely releases all dynamic memory allocated for container object X.
#define del(X)  _Generic(\
                        (X),\
                        string: __cmodules_string_del,\
                         array: __cmodules_array_del,\
                          list: __cmodules_list_del,\
                          fifo: __cmodules_fifo_del,\
                         stack: __cmodules_stack_del,\
                          dict: __cmodules_dict_del,\
                           set: __cmodules_set_del,\
                     list_node: __cmodules_list_node_del,\
                     fifo_node: __cmodules_fifo_node_del\
                    )(X)

void __cmodules_string_del (string);
void __cmodules_array_del  (array);
void __cmodules_list_del   (list);
void __cmodules_fifo_del   (fifo);
void __cmodules_stack_del  (stack);
void __cmodules_dict_del   (dict);
void __cmodules_set_del    (set);

void __cmodules_list_node_del(list_node);
void __cmodules_fifo_node_del(fifo_node);

// copy(X)
// Returns a deep copy of container object X.
#define copy(X) _Generic(\
                        (X),\
                        string: __cmodules_string_copy,\
                         array: __cmodules_array_copy,\
                          list: __cmodules_list_copy,\
                          fifo: __cmodules_fifo_copy,\
                         stack: __cmodules_stack_copy,\
                          dict: __cmodules_dict_copy,\
                           set: __cmodules_set_copy,\
                     list_node: __cmodules_list_node_copy,\
                     fifo_node: __cmodules_fifo_node_copy\
                    )(X)
string __cmodules_string_copy (string);
array  __cmodules_array_copy  (array);
list   __cmodules_list_copy   (list);
fifo   __cmodules_fifo_copy   (fifo);
stack  __cmodules_stack_copy  (stack);
dict   __cmodules_dict_copy   (dict);
set    __cmodules_set_copy    (set);

list_node __cmodules_list_node_copy(list_node);
fifo_node __cmodules_fifo_node_copy(fifo_node);


// add(X, ...)
// Appends/inserts value(s) into container object X.
#define add(X, ...) _Generic(\
                        (X),\
                        string: __cmodules_string_add,\
                         array: __cmodules_array_add,\
                          list: __cmodules_list_add,\
                          fifo: __cmodules_fifo_add,\
                         stack: __cmodules_stack_add,\
                          dict: __cmodules_dict_add,\
                           set: __cmodules_set_add\
                    )(X, __VA_ARGS__)
#define _array(A, ...) sizeof((typeof(A)[]){A, ## __VA_ARGS__})/sizeof(A), (raw_pointer*) (typeof(A)[]){A, ## __VA_ARGS__}
#define _node(T, V) #T, sizeof(T), (raw_pointer) (((intptr_t*)((T[]){V}))[0] & lsb(intptr_t, 8*sizeof(T))) // Helper macro for adding/searching elements, e.g. add(mylist, node(s64, 34))
#define _kv(K, V) (raw_pointer) K, icast(raw_pointer, V)

string __cmodules_string_add (string, raw_string);
array  __cmodules_array_add  (array,  size_t,      raw_pointer*);
list   __cmodules_list_add   (list,   raw_string,  size_t, raw_pointer);
fifo   __cmodules_fifo_add   (fifo,   raw_string,  size_t, raw_pointer);
stack  __cmodules_stack_add  (stack,  raw_string,  size_t, raw_pointer);
dict   __cmodules_dict_add   (dict,   raw_pointer, raw_pointer);
set    __cmodules_set_add    (set,    raw_string,  size_t, raw_pointer);

// rem(X,Y)
// Removes element at position/key Y from container object X.
#define rem(X,Y)    _Generic(\
                        (X),\
                        string: __cmodules_string_rem,\
                         array: __cmodules_array_rem,\
                          list: __cmodules_list_rem,\
                          dict: __cmodules_dict_rem,\
                           set: __cmodules_set_rem\
                    )(X, Y)

void __cmodules_string_rem (string, size_t);
void __cmodules_array_rem  (array,  size_t);
void __cmodules_list_rem   (list,   size_t);
void __cmodules_dict_rem   (dict,   raw_pointer);
void __cmodules_set_rem    (set,    size_t);

// clear(X)
// Removes all elements from container object X.
#define clear(X)    _Generic(\
                        (X),\
                        string: __cmodules_string_clear,\
                         array: __cmodules_array_clear,\
                          list: __cmodules_list_clear,\
                          fifo: __cmodules_fifo_clear,\
                         stack: __cmodules_stack_clear,\
                          dict: __cmodules_dict_clear,\
                           set: __cmodules_set_clear\
                    )(X)

void __cmodules_string_clear (string);
void __cmodules_array_clear  (array);
void __cmodules_list_clear   (list);
void __cmodules_fifo_clear   (fifo);
void __cmodules_stack_clear  (stack);
void __cmodules_dict_clear   (dict);
void __cmodules_set_clear    (set);

// contains(X,Y)
// Checks if element Y is contained in container object X.
#define contains(X, Y, ...) _Generic(\
                            (X),\
                            string: __cmodules_string_contains,\
                             array: __cmodules_array_contains,\
                              list: __cmodules_list_contains,\
                              fifo: __cmodules_fifo_contains,\
                             stack: __cmodules_stack_contains,\
                              dict: __cmodules_dict_contains,\
                               set: __cmodules_set_contains\
                            )(X, Y, ## __VA_ARGS__)

bool __cmodules_string_contains(string, string);
bool __cmodules_array_contains (array,  raw_pointer);
bool __cmodules_list_contains  (list,   raw_string, size_t, raw_pointer);
bool __cmodules_fifo_contains  (fifo,   raw_string, size_t, raw_pointer);
bool __cmodules_stack_contains (stack,  raw_string, size_t, raw_pointer);
bool __cmodules_dict_contains  (dict,   raw_pointer);
bool __cmodules_set_contains   (set,    raw_string, size_t, raw_pointer);

// index(X,e)
// Returns the position of element e in container object X, or -1 if the element is not in the object.
#define index(X, Y, ...) _Generic(\
                            (X),\
                            string: __cmodules_string_index,\
                             array: __cmodules_array_index,\
                              list: __cmodules_list_index,\
                               set: __cmodules_set_index\
                            )(X, Y, ## __VA_ARGS__)

size_t __cmodules_string_index(string, string);
size_t __cmodules_array_index (array,  raw_pointer);
size_t __cmodules_list_index  (list,   raw_string, size_t, raw_pointer);
size_t __cmodules_set_index   (set,    raw_string, size_t, raw_pointer);

// join(X,Y)
// Concatenates container object Y to object X. If the types do not match, nothing is done.
#define join(X,Y)   _Generic(\
                        (X),\
                        string: __cmodules_string_join,\
                         array: __cmodules_array_join,\
                          list: __cmodules_list_join,\
                          dict: __cmodules_dict_join,\
                           set: __cmodules_set_join\
                    )(X, Y)

string __cmodules_string_join (string, string);
array  __cmodules_array_join  (array,  array);
list   __cmodules_list_join   (list,   list);
dict   __cmodules_dict_join   (dict,   dict);
set    __cmodules_set_join    (set,    set);

// slice(X,a,b)
// Returns a new container object containing the slice of elements between a and b of object X.
#define slice(X,a,b) _Generic(\
                        (X),\
                        string: __cmodules_string_slice,\
                         array: __cmodules_array_slice,\
                          list: __cmodules_list_slice,\
                           set: __cmodules_set_slice\
                    )(X, a, b)

string __cmodules_string_slice(string, size_t, size_t);
array  __cmodules_array_slice (array,  size_t, size_t);
list   __cmodules_list_slice  (list,   size_t, size_t);
set    __cmodules_set_slice   (set,    size_t, size_t);

// reverse(X)
// Reverses the order of elements in container object X. Returns the object.
#define reverse(X)  _Generic(\
                        (X),\
                    string: __cmodules_string_reverse,\
                     array: __cmodules_array_reverse,\
                      list: __cmodules_list_reverse,\
                     stack: __cmodules_stack_reverse,\
                      fifo: __cmodules_fifo_reverse,\
                       set: __cmodules_set_reverse\
                    )(X)

string __cmodules_string_reverse(string);
array  __cmodules_array_reverse (array);
list   __cmodules_list_reverse  (list);
stack  __cmodules_stack_reverse (stack);
fifo   __cmodules_fifo_reverse  (fifo);
set    __cmodules_set_reverse   (set);

// sort(X)
// Sorts the container object X in increasing/alphabetical order. Returns X.
#define sort(X)  _Generic(\
                        (X),\
                    string: __cmodules_string_sort,\
                     array: __cmodules_array_sort,\
                      list: __cmodules_list_sort,\
                       set: __cmodules_set_sort\
                    )(X)

string __cmodules_string_sort(string);
array  __cmodules_array_sort (array);
list   __cmodules_list_sort  (list);
set    __cmodules_set_sort   (set);

// map(X,f)
// Applies function f to every element of container object X.
#define map(X,f)    _Generic(\
                        (X),\
                        string: __cmodules_string_map,\
                         array: __cmodules_array_map,\
                          list: __cmodules_list_map,\
                          dict: __cmodules_dict_map,\
                           set: __cmodules_set_map\
                    )(X, f)

string __cmodules_string_map(string, uchar (*f)(uchar));
array  __cmodules_array_map (array,  raw_pointer (*f)(raw_pointer));
list   __cmodules_list_map  (list,   raw_pointer (*f)(list_node));
dict   __cmodules_dict_map  (dict,   raw_pointer (*f)(dict_node));
set    __cmodules_set_map   (set,    raw_pointer (*f)(list_node));

// filter(X,f)
// Executes boolean function f using every element of X as an argument, then removes that element if f returns false.
#define filter(X,f)  _Generic(\
                        (X),\
                        string: __cmodules_string_filter,\
                         array: __cmodules_array_filter,\
                          list: __cmodules_list_filter,\
                          dict: __cmodules_dict_filter,\
                           set: __cmodules_set_filter\
                    )(X, f)

string __cmodules_string_filter(string, bool (*f)(uchar));
array  __cmodules_array_filter (array,  bool (*f)(raw_pointer));
list   __cmodules_list_filter  (list,   bool (*f)(list_node));
dict   __cmodules_dict_filter  (dict,   bool (*f)(dict_node));
set    __cmodules_set_filter   (set,    bool (*f)(list_node));

// reduce(X,f)
// Applies function f to the elements of container object X two-by-two, until a single value is obtained.
#define reduce(X,f) _Generic(\
                        (X),\
                        string: __cmodules_string_reduce,\
                         array: __cmodules_array_reduce,\
                          list: __cmodules_list_reduce,\
                          dict: __cmodules_dict_reduce,\
                           set: __cmodules_set_reduce\
                    )(X, f)

uchar       __cmodules_string_reduce(string, uchar (*f)(uchar, uchar));
raw_pointer __cmodules_array_reduce (array,  raw_pointer (*f)(raw_pointer, raw_pointer));
raw_pointer __cmodules_list_reduce  (list,   raw_pointer (*f)(raw_pointer, raw_pointer));
raw_pointer __cmodules_dict_reduce  (dict,   raw_pointer (*f)(raw_pointer, raw_pointer));
raw_pointer __cmodules_set_reduce   (set,    raw_pointer (*f)(raw_pointer, raw_pointer));

// getval(X,p)
// Returns the value corresponding to position/key p in container object X.
#define getval(X,p) _Generic(\
                        (X),\
                        string: __cmodules_string_getval,\
                         array: __cmodules_array_getval,\
                          list: __cmodules_list_getval,\
                          dict: __cmodules_dict_getval,\
                           set: __cmodules_set_getval\
                    )(X, p)

uchar       __cmodules_string_getval(string, size_t);
raw_pointer __cmodules_array_getval (array,  size_t);
raw_pointer __cmodules_list_getval  (list,   size_t);
raw_pointer __cmodules_dict_getval  (dict,   raw_pointer);
raw_pointer __cmodules_set_getval   (set,    size_t);

// setval(X,p,Y)
// Sets the value of the node corresponding to position/key p in container object X to Y.
#define setval(X,p,Y) _Generic(\
                        (X),\
                        string: __cmodules_string_setval,\
                         array: __cmodules_array_setval,\
                          list: __cmodules_list_setval,\
                          dict: __cmodules_dict_setval,\
                           set: __cmodules_set_setval\
                      )(X, p, ((raw_pointer*) ((typeof(Y)[]){Y}))[0])

void __cmodules_string_setval(string, size_t,       raw_pointer);
void __cmodules_array_setval (array,  size_t,       raw_pointer);
void __cmodules_list_setval  (list,   size_t,       raw_pointer);
void __cmodules_dict_setval  (dict,   raw_pointer,  raw_pointer);
void __cmodules_set_setval   (set,    size_t,       raw_pointer);

// get_type_size(s)
// Returns the size of the given type string s.
size_t __cmodules_get_type_size(raw_string);

/** for loops **/
#define foreach(e,X) for (list_node e = X->head; e != NULL; e = e->next)
#define for_length(i,X) for (long i = 0; i < X->len; i++)
#define for_reverse_length(i,X) for (long i = X->len-1; i >= 0; i--)
#define for_range(i,end,step) for (long i = 0; i <= end; i += step)
#define for_reverse_range(i,end,step) for (long i = end; i >= 0; i -= step);
#define for_keys(k,X) for (raw_pointer* k = X->keys; k != X->keys + X->len; k += 1)
#define for_pairs(p,X) for (dict_pair p = {X->keys[0], __cmodules_dict_getval(X, X->keys[0]), 0}; p.pos < X->len; __cmodules_dict_get_next_pair(&p,X))
void __cmodules_dict_get_next_pair(dict_pair* p, dict X) {
    (*p).pos += 1;
    if ((*p).pos == X->len)
        return;
    (*p).key = X->keys[(*p).pos];
    (*p).val = __cmodules_dict_getval(X,(*p).key);
}

/** Maximum Common Divisor (mcd) and Minimum Common Multiple (mcm) **/
#define mcd(x,y) _Generic(\
                          (x),\
                          s32: __cmodules_s32_mcd,\
                          s64: __cmodules_s64_mcd,\
                          u32: __cmodules_u32_mcd,\
                          u64: __cmodules_u64_mcd\
                          )(x,y)
#define mcm(x,y) ((x)*(y)/mcd(x,y))

/** Utility macros **/
size_t __cmodules_config_default_container_length = 2 << 13; // 8 KiB
#define default_container_length  __cmodules_config_default_container_length // 8 KiB by default
#define adapt_container_length(X) (((X)/default_container_length + 1) * default_container_length)

copy_function (*__cmodules_copy_function_hook)(raw_string) = NULL;
cmp_function  (*__cmodules_cmp_function_hook)(raw_string)  = NULL;
del_function  (*__cmodules_del_function_hook)(raw_string)  = NULL;
hash_function (*__cmodules_hash_function_hook)(raw_string) = NULL;

#include "src/types.c"

#endif //__CMODULES_TYPES__
