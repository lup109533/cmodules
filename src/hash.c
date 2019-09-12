#define hash(X, S) _Generic(\
                           (X),\
                           raw_string: __cmodules_hash_string,\
                               uchar*: __cmodules_hash_string,\
                                char*: __cmodules_hash_string,\
                               string: __cmodules_hash_string_object,\
                              default: __cmodules_hash_int\
                            )((raw_pointer) X, S)

#define __cmodules_polynomial_rolling_param 97 // Prime number closest to the usable characters in the ASCII character set

size_t __cmodules_hash_int(raw_pointer key, size_t bucket_size) {
    return ((intptr_t) key) % bucket_size;
}

size_t __cmodules_hash_string(raw_pointer _key, size_t bucket_size) {
    /** Polynomial rolling algorithm Sum(s[i]*p^i % bucket_size) for i = 0..len(s) where s is the string and p is **/
    /**  __cmodules_polynomial_rolling_param **/

    const raw_string key = (raw_string) _key;
    size_t key_length = strlen(key);
    size_t hash_value = key[0];
    size_t param      = __cmodules_polynomial_rolling_param;
    for (int i = 1; i < key_length; i++) {
        hash_value += (key[i] - ' ') * param; // space (32) is the first usable ASCII char
        hash_value %= bucket_size;
        param      *= param;
        param      %= bucket_size;
    }

    return hash_value;
}

size_t __cmodules_hash_string_object(raw_pointer key, size_t bucket_size) {
    return __cmodules_hash_string((raw_pointer) ((string) key)->val, bucket_size);
}

hash_function __cmodules_get_hash_function(const raw_string type) {
    if (strcmp(type, "raw_string") == 0 ||
        strcmp(type, "char*")      == 0 ||
        strcmp(type, "uchar*")     == 0)
            return &__cmodules_hash_string;
    else if (strcmp(type, "string") == 0)
            return &__cmodules_hash_string_object;
    else {
            if (__cmodules_hash_function_hook != NULL)
                return __cmodules_hash_function_hook(type);
            else
                return &__cmodules_hash_int;
    }
}
