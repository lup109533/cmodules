string __cmodules_string_map(string s, uchar (*f)(uchar)) {
    for (size_t i = 0; i < s->len; i++) {
        s->val[i] = f(s->val[i]);
    }

    return s;
}

array __cmodules_array_map (array a, void* (*f)(void*)) {
    for (size_t i = 0; i < a->len; i++) {
        raw_pointer mapped = f(__cmodules_array_getval(a, i));
        __cmodules_array_setval(a, i, mapped);
    }

    return a;
}

list __cmodules_list_map(list l, void* (*f)(list_node)) {
    list_node node = l->head;

    while (node != NULL) {
        node->val = f(node);
        node = node->next;
    }

    return l;
}

dict __cmodules_dict_map(dict d, void* (*f)(dict_node)) {
    if (d->len == 0)
        return d;

    for (size_t i = 0; i < d->len; i++) {
        size_t hsh = d->hash_key(d->keys[i], d->bucket_size);
        dict_node node = d->vals[hsh];
        node->val = f(node);
        while (node->has_collision) {
            node = node->next_for_key;
            node->val = f(node);
        }
    }

    return d;
}

set __cmodules_set_map(set s, void* (*f)(list_node)) {
    list_node node = s->head;

    while (node != NULL) {
        node->val = f(node);
        node = node->next;
    }

    return s;
}
