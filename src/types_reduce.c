uchar __cmodules_string_reduce(string s, uchar (*f)(uchar, uchar)) {
    if (s->len == 0)
        return 0;
    if (s->len == 1)
        return s->val[0];

    uchar result = s->val[s->len-1];
    for (size_t i = s->len-2; i != 0; i--) {
        result = f(result, s->val[i]);
    }

    return result;
}

void* __cmodules_array_reduce(array a, void* (*f)(void*, void*)) {
    if (a->len == 0)
        return NULL;
    if (a->len == 1)
        return ((void**) a->val)[0];

    void** alias  = (void**) a->val;
    void*  result = alias[a->len-1];
    for (size_t i = a->len-2; i != 0; i--) {
        result = f(result, alias[i]);
    }

    return result;
}

void* __cmodules_list_reduce(list l, void* (*f)(void*, void*)) {
    if (l->len == 0)
        return NULL;
    if (l->len == 1)
        return l->head->type_copy(l->head->val);

    list_node node   = l->head;
    void*     result = node->type_copy(node->val);
    node = node->next;

    while (node != NULL) {
        result = f(result, node->type_copy(node->val));
        node = node->next;
    }

    return result;
}

void* __cmodules_dict_reduce(dict d, void* (*f)(void*, void*)) {
    if (d->len == 0)
        return NULL;
    if (d->len == 1)
        return d->vals[d->hash_key(d->keys[0], d->bucket_size)]->val;

    dict_node node = __cmodules_dict_get_node(d, d->keys[0]);
    void* result = node->val;
    for (size_t i = 1; i < d->len; i++) {
        node = __cmodules_dict_get_node(d, d->keys[i]);
        result = f(result, node->val);
    }

    return result;
}

void* __cmodules_set_reduce(set l, void* (*f)(void*, void*)) {
    if (l->len == 0)
        return NULL;
    if (l->len == 1)
        return l->head->type_copy(l->head->val);

    list_node node   = l->head;
    void*     result = node->type_copy(node->val);
    node = node->next;

    while (node != NULL) {
        result = f(result, node->type_copy(node->val));
        node = node->next;
    }

    return result;
}
