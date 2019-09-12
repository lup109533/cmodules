#define cmp(X, Y) _Generic(\
                          (X),\
                          string: _Generic((Y),     string: __cmodules_string_cmp((string) X, (string) Y), default: (false)),\
                           array: _Generic((Y),      array: __cmodules_array_cmp ((array)  X, (array)  Y), default: (false)),\
                            list: _Generic((Y),       list: __cmodules_list_cmp  ((list)   X, (list)   Y), default: (false)),\
                           stack: _Generic((Y),      stack: __cmodules_stack_cmp ((stack)  X, (stack)  Y), default: (false)),\
                            fifo: _Generic((Y),       fifo: __cmodules_fifo_cmp  ((fifo)   X, (fifo)   Y), default: (false)),\
                            dict: _Generic((Y),       dict: __cmodules_dict_cmp  ((dict)   X, (dict)   Y), default: (false)),\
                             set: _Generic((Y),        set: __cmodules_set_cmp   ((set)    X, (set)    Y), default: (false)),\
                      raw_string: _Generic((Y), raw_string: (strcmp((raw_string) X, (raw_string) Y) == 0), default: (false)),\
                         default: ((X) == (Y))\
                          )

cmp_result  __cmodules_generic_cmp   (raw_pointer, raw_pointer);
cmp_result  __cmodules_raw_string_cmp(raw_pointer, raw_pointer);
cmp_result  __cmodules_string_cmp    (raw_pointer, raw_pointer);
cmp_result  __cmodules_array_cmp     (raw_pointer, raw_pointer);
cmp_result  __cmodules_list_cmp      (raw_pointer, raw_pointer);
cmp_result  __cmodules_stack_cmp     (raw_pointer, raw_pointer);
cmp_result  __cmodules_fifo_cmp      (raw_pointer, raw_pointer);
cmp_result  __cmodules_dict_cmp      (raw_pointer, raw_pointer);
cmp_result  __cmodules_set_cmp       (raw_pointer, raw_pointer);

cmp_function __cmodules_get_cmp_function(const raw_string type) {
    if (strcmp(type, "raw_string") == 0)
        return &__cmodules_raw_string_cmp;
    else if (strcmp(type, "string") == 0)
        return &__cmodules_string_cmp;
    else if (strcmp(type, "array") == 0)
        return &__cmodules_array_cmp;
    else if (strcmp(type, "list") == 0)
        return &__cmodules_list_cmp;
    else if (strcmp(type, "stack") == 0)
        return &__cmodules_stack_cmp;
    else if (strcmp(type, "fifo") == 0)
        return &__cmodules_fifo_cmp;
    else if (strcmp(type, "dict") == 0)
        return &__cmodules_dict_cmp;
    else if (strcmp(type, "set") == 0)
        return &__cmodules_list_cmp;
    else {
        if (__cmodules_cmp_function_hook != NULL)
            return __cmodules_cmp_function_hook(type);
        else
            return &__cmodules_generic_cmp;
    }
}

cmp_result  __cmodules_generic_cmp(raw_pointer X, raw_pointer Y) {
    cmp_result result;
    bool greater = X > Y;
    bool equal   = X == Y;

    result.eq = equal;
    result.ne = !equal;
    result.gt = greater;
    result.ge = greater || equal;
    result.lt = !greater && !equal;
    result.le = !greater;

    return result;
}

cmp_result __cmodules_raw_string_cmp(raw_pointer s1, raw_pointer s2) {
    size_t cmp = strcmp((raw_string) s1, (raw_string) s2);

    cmp_result result;

    result.eq = cmp == 0;
    result.ne = cmp != 0;
    result.ge = cmp >= 0;
    result.gt = cmp >  0;
    result.le = cmp <= 0;
    result.lt = cmp <  0;

    return result;
}

cmp_result __cmodules_string_cmp(raw_pointer X, raw_pointer Y) {
    string A = (string) X;
    string B = (string) Y;
    cmp_result result;
    s64  cmp;

    for (size_t i = 0; i < min(A->len, B->len); i++) {
        cmp = A->val[i] - B->val[i];
        if (cmp != 0)
            break;
    }
    if (cmp == 0 && A->len < B->len) {
        cmp = 1;
    }
    else if (cmp == 0 && A->len > B->len) {
        cmp = -1;
    }

    bool equal   = cmp == 0;
    bool greater = cmp > 0;

    result.eq = equal;
    result.ne = !equal;
    result.gt = greater;
    result.ge = greater || equal;
    result.lt = !greater && !equal;
    result.le = !greater;

    return result;
}

cmp_result __cmodules_array_cmp(raw_pointer X, raw_pointer Y) {
    array A = (array) X;
    array B = (array) Y;
    cmp_result result;
    bool equal = true;

    if (A->len != B->len || strcmp(A->type_id, B->type_id) != 0) {
        equal = false;
    }
    else {
        byte* x = A->val;
        byte* y = B->val;
        for (size_t i = 0; i < A->len; i++) {
            if (A->type_cmp((raw_pointer) x, (raw_pointer) y).ne) {
                equal = false;
                break;
            }
            x += A->type_size;
            y += B->type_size;
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}

cmp_result  __cmodules_list_cmp(raw_pointer X, raw_pointer Y) {
    list A = (list) X;
    list B = (list) Y;
    cmp_result result;
    bool equal = true;

    if (len(A) != len(B)) {
        equal = false;
    }
    else {
        list_node nodeX = A->head;
        list_node nodeY = B->head;
        for (size_t i = 0; i < len(A); i++) {
            if (strcmp(nodeX->type_id, nodeY->type_id) != 0) {
                equal = false;
                break;
            }

            cmp_function cmp_func = __cmodules_get_cmp_function(nodeX->type_id);
            if (cmp_func((raw_pointer) nodeX->val, (raw_pointer) nodeY->val).ne) {
                equal = false;
                break;
            }

            nodeX = nodeX->next;
            nodeY = nodeY->next;
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}

cmp_result  __cmodules_stack_cmp(raw_pointer X, raw_pointer Y) {
    stack A = (stack) X;
    stack B = (stack) Y;
    cmp_result result;
    bool equal = true;

    if (len(A) != len(B)) {
        equal = false;
    }
    else {
        list_node nodeX = A->head;
        list_node nodeY = B->head;
        for (size_t i = 0; i < len(A); i++) {
            if (strcmp(nodeX->type_id, nodeY->type_id) != 0) {
                equal = false;
                break;
            }

            cmp_function cmp_func = __cmodules_get_cmp_function(nodeX->type_id);
            if (cmp_func((raw_pointer) nodeX->val, (raw_pointer) nodeY->val).ne) {
                equal = false;
                break;
            }

            nodeX = nodeX->next;
            nodeY = nodeY->next;
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}

cmp_result  __cmodules_fifo_cmp(raw_pointer X, raw_pointer Y) {
    fifo A = (fifo) X;
    fifo B = (fifo) Y;
    cmp_result result;
    bool equal = true;

    if (len(A) != len(B)) {
        equal = false;
    }
    else {
        fifo_node nodeX = A->head;
        fifo_node nodeY = B->head;
        for (size_t i = 0; i < len(A); i++) {
            if (strcmp(nodeX->type_id, nodeY->type_id) != 0) {
                equal = false;
                break;
            }

            cmp_function cmp_func = __cmodules_get_cmp_function(nodeX->type_id);
            if (cmp_func((raw_pointer) nodeX->val, (raw_pointer) nodeY->val).ne) {
                equal = false;
                break;
            }

            nodeX = nodeX->next;
            nodeY = nodeY->next;
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}

cmp_result  __cmodules_dict_cmp(raw_pointer X, raw_pointer Y) {
    dict A = (dict) X;
    dict B = (dict) Y;
    cmp_result result;
    bool equal = true;

    if (len(A) != len(B) ||
        strcmp(A->key_type_id, B->key_type_id) != 0 ||
        strcmp(A->val_type_id, B->val_type_id) != 0 ||
        A->hash_key != B->hash_key) // Different hash functions may lead to different hashes for the same keys
    { equal = false; }
    else {
        // Do they have the same keys?
        for (size_t i = 0; i < len(A); i++) {
            void* keyX = A->keys[i];
            bool  is_in_Y = false;
            for (size_t j = 0; j < len(B); j++) {
                void* keyY = B->keys[j];
                if (A->key_type_cmp((raw_pointer) keyX, (raw_pointer) keyY).eq) {
                    is_in_Y = true;
                    break;
                }
            }
            if (!is_in_Y) {
                equal = false;
                break;
            }
        }

        // Are the keys mapped to the same values?
        for (size_t i = 0; i < len(A); i++) {
            raw_string key   = A->keys[i];
            dict_node  nodeX = __cmodules_dict_get_node(A, key);
            dict_node  nodeY = __cmodules_dict_get_node(B, key);
            if (A->val_type_cmp((raw_pointer) nodeX->val, (raw_pointer) nodeY->val).ne) {
                equal = false;
                break;
            }
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}

cmp_result  __cmodules_set_cmp(raw_pointer X, raw_pointer Y) {
    set A = (set) X;
    set B = (set) Y;
    cmp_result result;
    bool equal = true;

    if (len(A) != len(B)) {
        equal = false;
    }
    else {
        list_node nodeX = A->head;
        list_node nodeY = B->head;
        for (size_t i = 0; i < len(A); i++) {
            if (strcmp(nodeX->type_id, nodeY->type_id) != 0) {
                equal = false;
                break;
            }

            cmp_function cmp_func = __cmodules_get_cmp_function(nodeX->type_id);
            if (cmp_func((raw_pointer) nodeX->val, (raw_pointer) nodeY->val).ne) {
                equal = false;
                break;
            }

            nodeX = nodeX->next;
            nodeY = nodeY->next;
        }
    }

    result.eq = equal;
    result.ne = !equal;
    result.gt = false;
    result.ge = equal;
    result.lt = false;
    result.le = equal;

    return result;
}
