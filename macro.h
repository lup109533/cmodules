#ifndef __CMODULES_MACRO__
#define __CMODULES_MACRO__

/*** Macro definitions used throughout cmodules ***/

// Variadic macro helpers
#define VA_NARGS(...) ((int)(sizeof((int[]){__VA_ARGS__})/sizeof(int)))

// Structure value and length access
#define cval(x) x->val
#define clen(x) x->len

#endif
