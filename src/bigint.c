#include <stdarg.h>
bigint __cmodules_bigint_new(size_t size, size_t argnum, ...) {
    bigint result = alloc(__cmodules_bigint_struct);

    result->len   = size;
    result->block = alloc_array(s64,size);
    result->last  = result->block + size - 1;

    if (argnum > 0) {
        va_list args;
        va_start(args, argnum);
        s64 init = va_arg(args, int);
        result->block[0] = init;
        va_end(args);

        if (init < 0) {
            for (size_t i = 1; i < size; i++) {
                result->block[i] = -1;
            }
        }
    }

    return result;
}
