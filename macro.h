#ifndef __CMODULES_MACRO__
#define __CMODULES_MACRO__

/*** Macro definitions used throughout cmodules ***/

// Variadic macro helpers
#define VA_NARGS(...) ((int)(sizeof((int[]){__VA_ARGS__})/sizeof(int)))

// Structure value, length and size access
#define cval(x) x->val
#define clen(x) x->len
#define cmem(x) x->mem

// Common operations
#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)
#define clamp(x,a,b) ((x > a) ? a : ((x < b) ? b : x)

// Iterators and loops
typedef signed long __cmodules_loop_cnt_t;
#define for_range(cnt,target,step)        for(__cmodules_loop_cnt_t cnt = 0;     cnt < target; cnt += step)
#define for_reverse_range(cnt,start,step) for(__cmodules_loop_cnt_t cnt = start; cnt > 0;      cnt -= step)

#define foreach(elem,iter) _Generic((iter), \
                                    String: for(Char* elem = cval(iter); *elem != L'\0'; elem++) \
									  List: for(Node  elem = cbeg(iter); Node != NULL; elem = cnext(elem)) \
									)

#endif
