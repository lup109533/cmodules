string __cmodules_string_filter(string s, bool (*f)(uchar)) {
    size_t cur = 0;
    uchar* filtered_string = alloc_array(uchar, s->len);

    for (size_t i = 0; i < s->len; i++) {
        if (f(s->val[i])) {
            filtered_string[cur] = s->val[i];
            cur += 1;
        }
    }

    if (cur != s->len) {
        memset(s->val, 0, s->len * sizeof(uchar));
        for (size_t i = 0; i < cur; i++) {
            s->val[i] = filtered_string[i];
        }
        s->len = cur;
    }

    free(filtered_string);
    return s;
}

array  __cmodules_array_filter (array a,  bool (*f)(void*)) {
    size_t cur = 0;
    array  filtered_array = __cmodules_array_new(a->type_id, a->type_size, 0, 0, NULL);

    for (size_t i = 0; i < a->len; i++) {
        raw_pointer cur_val = __cmodules_array_getval(a, i);
        if (f(cur_val)) {
            __cmodules_array_setval(filtered_array, cur, cur_val);
            filtered_array->len += 1;
            cur += 1;
        }
    }

    if (cur != a->len) {
        __cmodules_array_clear(a);
        a->val = filtered_array->val;
        a->len = filtered_array->len;
        a->mem = filtered_array->mem;
    }
    else {
        __cmodules_array_clear(filtered_array);
    }

    free(filtered_array);
    return a;
}

list __cmodules_list_filter(list l, bool (*f)(list_node)) {
    list_node node = l->head;
    list_node next_node;
    list_node prev_node;

    while (node != NULL) {
        if (node == l->head) {
            if (f(node)) {
                l->head = node->next;
                next_node = l->head;
                __cmodules_list_node_del(node);
                node = next_node;
                l->len -= 1;
            }
            else {
                prev_node = node;
                node = node->next;
            }
        }
        else {
            if (f(node)) {
                next_node = node->next;
                prev_node->next = next_node;
                __cmodules_list_node_del(node);
                node = next_node;
                l->len -= 1;
            }
            else {
                prev_node = node;
                node = node->next;
            }
        }
    }

    if (l->len == 0)
        l->head = NULL;

    return l;
}

dict __cmodules_dict_filter(dict d, bool (*f)(dict_node)) {
    if (d->len == 0)
        return d;

    for (size_t i = 0; i < d->len; i++) {
        size_t hsh = d->hash_key(d->keys[i], d->bucket_size);
        dict_node node = d->vals[hsh];
        if (f(node) == false) {
            __cmodules_dict_rem(d, d->keys[i]);
        }
    }

    return d;
}

set __cmodules_set_filter(set l, bool (*f)(list_node)) {
    list_node node = l->head;
    list_node next_node;
    list_node prev_node;

    while (node != NULL) {
        if (node == l->head) {
            if (f(node)) {
                l->head = node->next;
                next_node = l->head;
                __cmodules_list_node_del(node);
                node = next_node;
                l->len -= 1;
            }
            else {
                prev_node = node;
                node = node->next;
            }
        }
        else {
            if (f(node)) {
                next_node = node->next;
                prev_node->next = next_node;
                __cmodules_list_node_del(node);
                node = next_node;
                l->len -= 1;
            }
            else {
                prev_node = node;
                node = node->next;
            }
        }
    }

    if (l->len == 0)
        l->head = NULL;

    return l;
}
