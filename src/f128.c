#include <stdio.h>

struct __cmodules_F128_library F128 = {
    .get = &__cmodules_f128_get,
};

f128 __cmodules_f128_get(f64 f) {
    f128  result;
    char* fpnt = (char*) &f;

    for_reverse_range(i,8,1) {
        printf("%02X",fpnt[i]);
    }
    printf("\n");

    result.sign  = ((u8) fpnt[0]) >> 7;
    result.exp   = ((((u16) fpnt[0]) & 0x7F) << 8) | ((u16) fpnt[1]);
    result.mant0 = (((u16) fpnt[2]) << 8) | ((u16) fpnt[3]);
    result.mant1 = (((u32) fpnt[4]) << 24) | (((u32) fpnt[5]) << 16) | (((u32) fpnt[6]) << 8) | ((u32) fpnt[7]);
    result.mant2 = 0;
    result.mant3 = 0;

    return result;
}

#ifdef __CMODULES_BIT__
f80_unpacked __cmodules_unpack_f80(f80 f) {
    f80_unpacked result;
    u64 u0;
    u16 u1;
    char* fpnt  = (char*) &f;
    char* u0pnt = (char*) &u0;
    char* u1pnt = (char*) &u1;

    for_range(i,2,1) {
        u1pnt[i] = fpnt[i];
    }
    for_range(i,8,1) {
        u0pnt[i] = fpnt[i+2];
    }

    result.sign     = u1 >> 15;
    result.exponent = u1 & 0x7FFF;
    result.mantissa = u0;

    return result;
}

f128_unpacked __cmodules_unpack_f128(f128 f) {
    f128_unpacked result;
    u64 u0;
    u64 u1;
    u16 u2;
    char* fpnt  = (char*) &f;
    char* u0pnt = (char*) &u0;
    char* u1pnt = (char*) &u1;
    char* u2pnt = (char*) &u2;

    for_range(i,2,1) {
        u2pnt[i] = fpnt[i];
    }
    for_range(i,6,1) {
        u1pnt[i+2] = fpnt[i+2];
    }
    for_range(i,8,1) {
        u0pnt[i] = fpnt[i+8];
    }
    result.sign      = u2 >> 15;
    result.exponent  = u2 & 0x7FFF;
    result.mantissah = u1 & 0x0000FFFFFFFFFFFF;
    result.mantissal = u0;

    return result;
}

f80 __cmodules_pack_f80(f80_unpacked inp) {
    u16   sign_and_exp = (inp.sign << 15) | inp.exponent;
    u64   mant         = inp.mantissa;
    f80   result;
    char* resultpnt    = (char*) &result;

    for_range(i,2,1) {
        resultpnt[i] = ((char*)&sign_and_exp)[i];
    }
    for_range(i,8,1) {
        resultpnt[i+2] = ((char*)&mant)[i];
    }

    return result;
}
f128 __cmodules_pack_f128(f128_unpacked inp) {
    u16   sign_and_exp = (inp.sign << 15) | inp.exponent;
    u64   manth        = inp.mantissah & 0x0000FFFFFFFFFFFF;
    u64   mantl        = inp.mantissal;
    f128  result;
    char* resultpnt    = (char*) &result;

    for_range(i,2,1) {
        resultpnt[i] = ((char*)&sign_and_exp)[i];
    }
    for_range(i,6,1) {
        resultpnt[i+2] = ((char*)&manth)[i+2];
    }
    for_range(i,8,1) {
        resultpnt[i+8] = ((char*)&mantl)[i];
    }

    return result;
}
#endif
