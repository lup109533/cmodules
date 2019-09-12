void __cmodules_string_setval(string s, size_t pos, raw_pointer val) {
    if (pos > s->len-1)
        pos = s->len-1;

    s->val[pos] = (uchar) ((intptr_t) val);
}

void __cmodules_array_setval(array a, size_t pos, raw_pointer val) {
    if (pos > a->len-1)
        pos = a->len-1;

    switch (a->type_size) {
        case 1:
            a->val[pos] = (byte) ((intptr_t) a->type_copy(val));
            break;

        case 2:
            ((int16_array) a)->val[pos] = (s16) ((intptr_t) a->type_copy(val));
            break;

        case 4:
            if (sizeof(raw_pointer) == 4) {
                a->type_del((raw_pointer) ((intptr_t) ((int32_array) a)->val[pos]));
            }
            ((int32_array) a)->val[pos] = (s32) ((intptr_t) a->type_copy(val));
            break;

        case 8:
            if (sizeof(raw_pointer) == 8) {
                a->type_del((raw_pointer) ((intptr_t) ((int64_array) a)->val[pos]));
            }
            ((int64_array) a)->val[pos] = (s64) ((intptr_t) a->type_copy(val));
            break;

        default:
            ((pointer_array) a)->val[pos] = a->type_copy(val);
            break;
    }
}

void __cmodules_list_setval(list l, size_t pos, raw_pointer val) {
    if (l->len == 0)
        return;

    if (pos > l->len-1)
        pos = l->len-1;

    list_node node = l->head;
    for (size_t i = 0; i < pos; i++) {
        node = node->next;
    }

    raw_pointer val_copy = node->type_copy(val);
    node->val = val_copy;
}

void __cmodules_dict_setval(dict d, raw_pointer key, raw_pointer val) {
    if (!__cmodules_dict_has_key(d, key))
        return;

    raw_pointer val_copy = d->val_type_copy(val);
    dict_node node = __cmodules_dict_get_node(d, key);
    node->val = val_copy;
}

void __cmodules_set_setval(set s, size_t pos, raw_pointer val) {
    if (s->len == 0)
        return;

    if (pos > s->len-1)
        pos = s->len-1;

    list_node node = s->head;
    for (size_t i = 0; i < pos; i++) {
        node = node->next;
    }
    raw_pointer val_copy = node->type_copy(val);
    node->val = val_copy;
}
