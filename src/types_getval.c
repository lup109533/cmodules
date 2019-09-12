uchar __cmodules_string_getval(string s, size_t pos) {
    if (pos > s->len-1)
        pos = s->len-1;

    return s->val[pos];
}

raw_pointer __cmodules_array_getval(array a, size_t pos) {
    if (pos > a->len-1)
        pos = a->len-1;

    switch (a->type_size) {
        case 1:
            return (raw_pointer) ((intptr_t) a->val[pos]);
        case 2:
            return (raw_pointer) ((intptr_t) ((int16_array) a)->val[pos]);
        case 4:
            return (raw_pointer) ((intptr_t) ((int32_array) a)->val[pos]);
        case 8:
            return (raw_pointer) ((intptr_t) ((int64_array) a)->val[pos]);
        default:
            return (raw_pointer) ((pointer_array) a)->val[pos];
    }
}

raw_pointer __cmodules_list_getval(list l, size_t pos) {
    if (l->len == 0)
        return NULL;

    if (pos > l->len-1)
        pos = l->len-1;

    list_node node = l->head;
    for (size_t i = 0; i < pos; i++) {
        node = node->next;
    }

    return node->type_copy(node->val);
}

raw_pointer __cmodules_dict_getval(dict d, raw_pointer key) {
    if (!__cmodules_dict_has_key(d, key))
        return NULL;

    return d->val_type_copy(__cmodules_dict_get_node(d, key)->val);
}

raw_pointer __cmodules_set_getval(set s, size_t pos) {
    if (s->len == 0)
        return NULL;

    if (pos > s->len-1)
        pos = s->len-1;

    list_node node = s->head;
    for (size_t i = 0; i < pos; i++) {
        node = node->next;
    }

    return node->type_copy(node->val);
}
