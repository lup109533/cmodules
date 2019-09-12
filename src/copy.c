raw_pointer __cmodules_trivial_copy(raw_pointer x) {
    return x;
}

raw_pointer __cmodules_raw_string_copy(raw_pointer x) {
    size_t     length = strlen((raw_string) x);
    raw_string result = alloc_array(char, length);
    strcpy(result, (raw_string) x);
    return (raw_pointer) result;
}

raw_pointer __cmodules_string_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_string_copy((string) x);
}

raw_pointer __cmodules_array_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_array_copy((array) x);
}

raw_pointer __cmodules_list_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_list_copy((list) x);
}

raw_pointer __cmodules_stack_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_stack_copy((stack) x);
}

raw_pointer __cmodules_fifo_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_fifo_copy((fifo) x);
}

raw_pointer __cmodules_dict_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_dict_copy((dict) x);
}

raw_pointer __cmodules_set_copy_wrapper(raw_pointer x) {
    return (raw_pointer) __cmodules_set_copy((set) x);
}

copy_function __cmodules_get_copy_function(const raw_string type) {
    if (strcmp(type, "raw_string") == 0 ||
        strcmp(type, "uchar*")     == 0 ||
        strcmp(type, "char*")      == 0)
        return &__cmodules_raw_string_copy;
    else if (strcmp(type, "string") == 0)
        return &__cmodules_string_copy_wrapper;
    else if (strcmp(type, "array") == 0)
        return &__cmodules_array_copy_wrapper;
    else if (strcmp(type, "list") == 0)
        return &__cmodules_list_copy_wrapper;
    else if (strcmp(type, "stack") == 0)
        return &__cmodules_stack_copy_wrapper;
    else if (strcmp(type, "fifo") == 0)
        return &__cmodules_fifo_copy_wrapper;
    else if (strcmp(type, "dict") == 0)
        return &__cmodules_dict_copy_wrapper;
    else if (strcmp(type, "set") == 0)
        return &__cmodules_set_copy_wrapper;
    else {
        if (__cmodules_copy_function_hook != NULL)
            return __cmodules_copy_function_hook(type);
        else
            return &__cmodules_trivial_copy;
    }
}
