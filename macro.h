#ifndef __CMODULES_MACRO__
#define __CMODULES_MACRO__

/*** Macro definitions used throughout cmodules ***/

#define cmodules_config(conf, val) __cmodules_config_ ## conf = (val)

// Useful shorthands
#define elif else if
#define and &&
#define or  ||
#define not !
#define alloc(T)    malloc(sizeof(T))
#define alloc_array(T,N) calloc(sizeof(T), (N))
#define alloc_string(N) alloc_array(char, (N)+1)

// Variadic macro helpers
#define __VA_NARGS__(...)  ((int)(sizeof((int[]){__VA_ARGS__})/sizeof(int)))     // for scalars
#define __VA_NPARGS__(...) ((int)(sizeof((void*[]){__VA_ARGS__})/sizeof(void*))) // for pointers

// Common operations
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#define clamp(x,a,b) ((x) > (a) ? (a) : ((x) < (b) ? (b) : (x))

// Bit manipulation
#define _0b(n) __cmodules__0b(#n, sizeof(#n))
unsigned __cmodules__0b(const char* bit, unsigned len) {
    unsigned result = 0;
    for (unsigned i = len-1; (i+1) > i; i--) {
        if (bit[i] == '0') {
            result <<= 1;
        }
        elif (bit[i] == '1') {
            result <<= 1;
            result += 1;
        }
    }

    return result;
}
#define lsb(T,n) ((T) __cmodules_lsbs(n))
unsigned __cmodules_lsbs(unsigned n) {
    if (n == 0)
        return 0;

    unsigned result = 1;
    for (unsigned i = 1; i < n && i < 8*sizeof(void*); i++) {
        result <<= 1;
        result += 1;
    }

    return result;
}
#define msb(T,n) (T) ~(lsb(T, max(8*sizeof(T) - n, 0)))

// Casts
// Interpret cast(T, X)
// Interprets the given binary data X as type T.
#define icast(T, X) (((T*) ((typeof(X)[]){X}))[0])

#define bcast(T, X) (((T)(X)) & lsb(T, 8*sizeof(typeof(X))))

#endif
