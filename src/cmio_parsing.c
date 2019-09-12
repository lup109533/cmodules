s8 __cmodules_parse_s8(string s) {
    if (sizeof(intptr_t) == sizeof(s32)) {
        return (s8) __cmodules_parse_s32(s);
    }
    else {
        return (s8) __cmodules_parse_s64(s);
    }
}
s16 __cmodules_parse_s16(string s) {
    if (sizeof(intptr_t) == sizeof(s32)) {
        return (s16) __cmodules_parse_s32(s);
    }
    else {
        return (s16) __cmodules_parse_s64(s);
    }
}
s32 __cmodules_parse_s32(string s) {
    s32 parse_bin(string s, long init) {
        s32 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char == 0 || cur_char == 1) {
                num *= 2;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s32 parse_oct(string s, long init) {
        s32 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char >= 0 && cur_char <= 7) {
                num *= 8;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s32 parse_dec(string s, long init) {
        s32 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char >= 0 && cur_char <= 9) {
                num *= 10;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s32 parse_hex(string s, long init) {
        s32 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i];
            if (cur_char >= '0' && cur_char <= '9') {
                num *= 16;
                num += cur_char - '0';
            }
            elif (cur_char >= 'A' && cur_char <= 'F') {
                num *= 16;
                num += cur_char - 'A' + 10;
            }
            elif (cur_char >= 'a' && cur_char <= 'f') {
                num *= 16;
                num += cur_char - 'a' + 10;
            }
            else {
                return 0;
            }
        }
        return num;
    }

    if (len(s) == 0)
        return 0;

    long init = 0;
    bool negative = false;
    s32 result;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Get sign, if any
    if (s->val[init] == '+') {
        init += 1;
    }
    elif (s->val[init] == '-') {
        init += 1;
        negative = true;
    }

    // Parse base prefixes, if any, and choose parser
    if (len(s) - init > 2) {
        uchar first_char = s->val[init];
        uchar base_id    = s->val[init+1];
        if (first_char == '0') {
            switch (base_id) {
                case 'b':
                case 'B':
                    result = parse_bin(s, init+2);
                    break;

                case 'o':
                case 'O':
                    result = parse_oct(s, init+2);
                    break;

                case 'd':
                case 'D':
                    result = parse_dec(s, init+2);
                    break;

                case 'x':
                case 'X':
                    result = parse_hex(s, init+2);
                    break;

                default:
                    result = parse_dec(s, init);
                    break;
            }
        }
        else {
            result = parse_dec(s, init);
        }
    }
    else {
        result = parse_dec(s, init);
    }

    return (negative) ? -result : result;
}
s64 __cmodules_parse_s64(string s) {
    s64 parse_bin(string s, long init) {
        s64 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char == 0 || cur_char == 1) {
                num *= 2;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s64 parse_oct(string s, long init) {
        s64 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char >= 0 && cur_char <= 7) {
                num *= 8;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s64 parse_dec(string s, long init) {
        s64 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i] - '0';
            if (cur_char >= 0 && cur_char <= 9) {
                num *= 10;
                num += cur_char;
            }
            else {
                return 0;
            }
        }
        return num;
    }
    s64 parse_hex(string s, long init) {
        s64 num = 0;
        for (long i = init; i < s->len; i++) {
            uchar cur_char = s->val[i];
            if (cur_char >= '0' && cur_char <= '9') {
                num *= 16;
                num += cur_char - '0';
            }
            elif (cur_char >= 'A' && cur_char <= 'F') {
                num *= 16;
                num += cur_char - 'A' + 10;
            }
            elif (cur_char >= 'a' && cur_char <= 'f') {
                num *= 16;
                num += cur_char - 'a' + 10;
            }
            else {
                return 0;
            }
        }
        return num;
    }

    if (len(s) == 0)
        return 0;

    long init = 0;
    bool negative = false;
    s64 result;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Get sign, if any
    if (s->val[init] == '+') {
        init += 1;
    }
    elif (s->val[init] == '-') {
        init += 1;
        negative = true;
    }

    // Parse base prefixes, if any, and choose parser
    if (len(s) - init > 2) {
        uchar first_char = s->val[init];
        uchar base_id    = s->val[init+1];
        if (first_char == '0') {
            switch (base_id) {
                case 'b':
                case 'B':
                    result = parse_bin(s, init+2);
                    break;

                case 'o':
                case 'O':
                    result = parse_oct(s, init+2);
                    break;

                case 'd':
                case 'D':
                    result = parse_dec(s, init+2);
                    break;

                case 'x':
                case 'X':
                    result = parse_hex(s, init+2);
                    break;

                default:
                    result = parse_dec(s, init);
                    break;
            }
        }
        else {
            result = parse_dec(s, init);
        }
    }
    else {
        result = parse_dec(s, init);
    }

    return (negative) ? -result : result;
}

u8 __cmodules_parse_u8(string s) {
    return (u8) __cmodules_parse_s8(s);
}
u16 __cmodules_parse_u16(string s) {
    return (u16) __cmodules_parse_s16(s);
}
u32 __cmodules_parse_u32(string s) {
    return (u32) __cmodules_parse_s32(s);
}
u64 __cmodules_parse_u64(string s) {
    return (u64) __cmodules_parse_s64(s);
}

#include <stdlib.h>
f32 __cmodules_parse_f32(string s) {
    raw_string str = alloc_string(s->len);
    for_length(i,s) {
        str[i] = (char) s->val[i];
    }

    f32 result = (f32) atof(str);
    free(str);
    return result;
}

f64 __cmodules_parse_f64(string s) {
    raw_string str = alloc_string(s->len);
    for_length(i,s) {
        str[i] = (char) s->val[i];
    }

    f64 result = (f64) atof(str);
    free(str);
    return result;
}

cf64 __cmodules_parse_complex(string s) {
    raw_string str = alloc_string(s->len);
    cf64 result;
    f64 real_part = 0.0;
    f64 imag_part = 0.0;
    bool got_real = false;
    bool got_imag = false;
    for (long i = 0; i < s->len; i++) {
        if (s->val[i] == '+' || s->val[i] == '-') {
            memset(str, 0, sizeof(uchar)*(s->len-1));
            for (long j = 0; j < i; j++) {
                str[j] = (char) s->val[j];
            }
            real_part = (f64) atof(str);
            got_real = true;
        }
        elif (s->val[i] == 'i' || s->val[i] == 'I') {
            memset(str, 0, sizeof(uchar)*(s->len-1));
            for (long j = 0; j < s->len && s->val[j] != ' '; j++) {
                if (s->val[j] != 'i' && s->val[j] != 'I' && s->val[j] != '*')
                    str[j] = (char) s->val[j];
            }
            imag_part = (f64) atof(str);
            got_imag = true;
        }

        if (got_real && got_imag)
            break;
    }

    free(str);
    result = real_part + imag_part*I;
    return result;
}

raw_string __cmodules_parse_raw_string(string s) {
    raw_string str;

    if (s->len == 0) {
        str = alloc_string(0);
        return str;
    }

    long init = 0;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Check bounds
    long beg = init + 1;
    long end = s->len;
    if (s->val[init] != '\'') {
        str = alloc_string(0);
        return str;
    }
    for (long i = beg; i < s->len; i++) {
        if (s->val[i] == '\'' && s->val[i+1] != '\'' && s->val[max(0,i-1)] != '\'') {
            end = i;
            break;
        }
    }
    if (end == s->len) {
        str = alloc_string(0);
        return str;
    }

    // Get proper string size
    size_t str_size = 0;
    for (long i = beg; i < end; i++) {
        if (!(s->val[i] == '\'' && s->val[i+1] == '\'')) {
            str_size += code_point_size(s->val[i]);
        }
    }
    str = alloc_string(str_size);

    // Write string
    size_t cur = 0;
    for (long i = beg; i < end; i++) {
        if (!(s->val[i] == '\'' && s->val[i+1] == '\'')) {
            code_point_to_utf8(str + cur, s->val[i]);
            cur += code_point_size(s->val[i]);
        }
    }

    return str;
}

string __cmodules_parse_string(string s) {
    string str = new(string);

    if (s->len == 0) {
        return str;
    }

    long init = 0;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Check bounds
    long beg = init + 1;
    long end = s->len;
    if (s->val[init] != '"') {
        return str;
    }
    for (long i = beg; i < s->len; i++) {
        if (s->val[i] == '"' && s->val[i+1] != '"' && s->val[max(0,i-1)] != '"') {
            end = i;
            break;
        }
    }
    if (end == s->len) {
        return str;
    }

    // Write string
    for (long i = beg; i < end; i++) {
        if (!(s->val[i] == '"' && s->val[i+1] == '"')) {
            str->val[str->len] = s->val[i];
            str->len += 1;
        }
    }

    return str;
}
array __cmodules_parse_array(string s) {
    if (s->len == 0) {
        if (sizeof(intptr_t) == sizeof(s32))
            return new(array, s32);
        else
            return new(array, s64);
    }

    long init = 0;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Check boundaries
    if (s->val[init] != '(') {
        if (sizeof(intptr_t) == sizeof(s32))
            return new(array, s32);
        else
            return new(array, s64);
    }
    size_t end;
    for (long i = s->len-1; i >= 0; i--) {
        if (s->val[i] == ')') {
            end = i;
            break;
        }
        elif (s->val[i] == ' ') {
            continue;
        }
        else {
            if (sizeof(intptr_t) == sizeof(s32))
                return new(array, s32);
            else
                return new(array, s64);
        }
    }

    // Separate array elements
    raw_string str = alloc_string(s->len);
    array elems = new(array, string);
    size_t cur = 0;
    for (long i = init; i <= end; i++) {
        if (s->val[i] == ',' || i == end) {
            string new_elem = new(string, str);
            add(elems, _array(new_elem));
            memset(str, 0, s->len);
            cur = 0;
        }
        else {
            str[cur] = (char) s->val[i];
            cur += 1;
        }
    }
    free(str);

    // Parse elements
    raw_string type_str = __cmodules_infer_type((string) getval(elems,0));
    size_t type_size = __cmodules_get_type_size(type_str);
    array result = __cmodules_array_new(type_str, type_size, elems->len, 0, NULL);

    if (strcmp(type_str, "u8") == 0 || strcmp(type_str, "s8") == 0 || strcmp(type_str, "byte") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_s8(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "u16") == 0 || strcmp(type_str, "s16") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_s16(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "u32") == 0 || strcmp(type_str, "s32") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_s32(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "u64") == 0 || strcmp(type_str, "s64") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_s64(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "f32") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_f32(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "f64") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_f64(getval(elems, i)));
        }
    }
    elif (strcmp(type_str, "array") == 0) {
        for_length(i, elems) {
            setval(result, i, __cmodules_parse_array(getval(elems, i)));
        }
    }
//    elif (strcmp(type_str, "list") == 0) {
//        for_length(i, elems) {
//            setval(result, i, __cmodules_parse_list(getval(elems, i)));
//        }
//    }
//    elif (strcmp(type_str, "dict") == 0) {
//        for_length(i, elems) {
//            setval(result, i, __cmodules_parse_dict(getval(elems, i)));
//        }
//    }
    else {
        if (sizeof(intptr_t) == sizeof(s32)) {
            for_length(i, elems) {
                setval(result, i, __cmodules_parse_s32(getval(elems, i)));
            }
        }
        else {
            for_length(i, elems) {
                setval(result, i, __cmodules_parse_s64(getval(elems, i)));
            }
        }
    }

    del(elems);
    free(type_str);
    return result;
}
list       __cmodules_parse_list      (string);
dict       __cmodules_parse_dict      (string);

raw_string __cmodules_infer_type(string s) {
    if (s->len == 0)
        return "notype";

    long init = 0;
    long cur;

    // Skip spaces
    for (; s->val[init] == ' '; init++);

    // Check for characteristic type tells
    switch (s->val[init]) {
        case '\'':
            for (cur = init+1; cur < s->len && s->val[cur] != '\''; cur++);
            if (cur == s->len) {
                return "notype";
            }
            else {
                if (cur - init == 2) {
                    return "char";
                }
                else {
                    return "raw_string";
                }
            }

        case '\"':
            for (cur = init+1; cur < s->len && s->val[cur] != '\"'; cur++);
            if (cur == s->len) {
                return "notype";
            }
            else {
                if (cur - init == 2) {
                    return "uchar";
                }
                else {
                    return "string";
                }
            }

        case '(':
            for (cur = init+1; cur < s->len && s->val[cur] != ')'; cur++);
            if (cur == s->len) {
                return "notype";
            }
            else {
                return "array";
            }

        case '[':
            for (cur = init+1; cur < s->len && s->val[cur] != ']'; cur++);
            if (cur == s->len) {
                return "notype";
            }
            else {
                return "list";
            }

        case '{':
            for (cur = init+1; cur < s->len && s->val[cur] != '}'; cur++);
            if (cur == s->len) {
                return "notype";
            }
            else {
                return "dict";
            }

        default:
            break;
    }

    for (cur = init; cur < s->len && s->val[cur] != '+' && s->val[cur] != '-' && s->val[cur] != 'i' && s->val[cur] != 'I'; cur++);
    if (cur != s->len) {
        return "cf64";
    }

    for (cur = init; cur < s->len && s->val[cur] != '.'; cur++);
    if (cur == s->len) {
        for (cur = init; cur < s->len && (s->val[cur] != 'e' && s->val[cur] != 'E'); cur++);
        if (cur == s->len)
            return (sizeof(intptr_t) == 4) ? "s32" : "s64";
        else
            return "f64";
    }
    else {
        return "f64";
    }

    return (sizeof(intptr_t) == 4) ? "s32" : "s64";
}
