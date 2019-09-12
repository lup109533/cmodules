#include <stdlib.h>

byte* __cmodules_bit_to_byte_array (void* pnt, size_t s) {
	byte* result = malloc(s);
	for_range(i,s,1) {
        result[i] = ((byte*) pnt)[i];
	}

	return result;
}

u64 __cmodules_bit__0b(const char* s, size_t m) {
	u64 result = 0;
	size_t n   = 0;
	char   cur;
	m -= 2;

	while ((cur = s[n]) != '\0' && n < 64) {
		if (cur == '1') {
			result += 1 << (m - n);
		}
		n++;
	}

	return result;
}

f32_unpacked __cmodules_unpack_f32(f32 f) {
    f32_unpacked result;
    u32          u;
    char* fpnt = (char*) &f;
    char* upnt = (char*) &u;

    for_range(i,4,1) {
        upnt[i] = fpnt[i];
    }
    result.sign     = u >> 31;
    result.exponent = (u & 0x7F800000) >> 23;
    result.mantissa = u & 0x007FFFFF;

    return result;
}
f64_unpacked __cmodules_unpack_f64(f64 f) {
    f64_unpacked result;
    u64          u;
    char* fpnt = (char*) &f;
    char* upnt = (char*) &u;

    for_range(i,8,1) {
        upnt[i] = fpnt[i];
    }
    result.sign     = u >> 63;
    result.exponent = (u & 0x7FF0000000000000) >> 52;
    result.mantissa = u & 0x000FFFFFFFFFFFFF;

    return result;
}
f32 __cmodules_pack_f32(f32_unpacked inp) {
    f32 f;
    u32 u;
    char* fpnt = (char*) &f;
    char* upnt = (char*) &u;

    u = (inp.sign << 31) | (inp.exponent << 23) | inp.mantissa;

    for_range(i,4,1) {
        fpnt[i] = upnt[i];
    }

    return f;
}
f64 __cmodules_pack_f64(f64_unpacked inp) {
    f64 f;
    u64 u;
    char* fpnt = (char*) &f;
    char* upnt = (char*) &u;

    u = ((u64)inp.sign << 63) | ((u64)inp.exponent << 52) | (u64)inp.mantissa;

    for_range(i,8,1) {
        fpnt[i] = upnt[i];
    }

    return f;
}
