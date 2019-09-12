/** add **/
fract32 __cmodules_fract32_add(fract32 x, fract32 y) {
    s32 num = x.num * y.den + y.num * x.den;
    s32 den = x.den * y.den;
    s32 div = mcd(num, den);

    return (fract32) {num/div, den/div};
}

fract64 __cmodules_fract64_add(fract64 x, fract64 y) {
    s64 num = x.num * y.den + y.num * x.den;
    s64 den = x.den * y.den;
    s64 div = mcd(num, den);

    return (fract64) {num/div, den/div};
}

/** inv **/
fract32 __cmodules_fract32_inv(fract32 x) {
    return (fract32) {x.den, x.num};
}

fract64 __cmodules_fract64_inv(fract64 x) {
    return (fract64) {x.den, x.num};
}

 /** mul **/
fract32 __cmodules_fract32_mul(fract32 x, fract32 y) {
    s32 num = x.num * y.num;
    s32 den = x.den * y.den;
    s32 div = mcd(num, den);

    return (fract32) {num/div, den/div};
}

fract64 __cmodules_fract64_mul(fract64 x, fract64 y) {
    s64 num = x.num * y.num;
    s64 den = x.den * y.den;
    s64 div = mcd(num, den);

    return (fract64) {num/div, den/div};
}
