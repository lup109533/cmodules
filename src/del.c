void __cmodules_trivial_del(raw_pointer x) {
    return;
}

void __cmodules_pointer_del(raw_pointer x) {
    free((void*)x);
}

void __cmodules_string_del_wrapper(raw_pointer x) {
    __cmodules_string_del((string) x);
}

void __cmodules_array_del_wrapper(raw_pointer x) {
    __cmodules_array_del((array) x);
}

void __cmodules_list_del_wrapper(raw_pointer x) {
    __cmodules_list_del((list) x);
}

void __cmodules_stack_del_wrapper(raw_pointer x) {
    __cmodules_stack_del((stack) x);
}

void __cmodules_fifo_del_wrapper(raw_pointer x) {
    __cmodules_fifo_del((fifo) x);
}

void __cmodules_dict_del_wrapper(raw_pointer x) {
    __cmodules_dict_del((dict) x);
}

void __cmodules_set_del_wrapper(raw_pointer x) {
    __cmodules_set_del((set) x);
}

del_function __cmodules_get_del_function(const raw_string type) {
    if (strcmp(type, "raw_string")  == 0 ||
        strcmp(type, "raw_pointer") == 0)
        return &__cmodules_pointer_del;
    else if (strcmp(type, "string") == 0)
        return &__cmodules_string_del_wrapper;
    else if (strcmp(type, "array") == 0)
        return &__cmodules_array_del_wrapper;
    else if (strcmp(type, "list") == 0)
        return &__cmodules_list_del_wrapper;
    else if (strcmp(type, "stack") == 0)
        return &__cmodules_stack_del_wrapper;
    else if (strcmp(type, "fifo") == 0)
        return &__cmodules_fifo_del_wrapper;
    else if (strcmp(type, "dict") == 0)
        return &__cmodules_dict_del_wrapper;
    else if (strcmp(type, "set") == 0)
        return &__cmodules_set_del_wrapper;
    else {
        if (__cmodules_del_function_hook != NULL)
            return __cmodules_del_function_hook(type);
        else
            return &__cmodules_trivial_del;
    }
}
