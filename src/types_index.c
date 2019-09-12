size_t __cmodules_string_index(string s, string e) {
    if (!(s->len > e->len))
        return -1;

    for (size_t i = 0; i < s->len - e->len; i++) {
        if (memcmp(s->val + i, e->val, e->len * sizeof(uchar)) == 0)
            return i;
    }

    return -1;
}

size_t __cmodules_array_index(array a, void* e) {
    if (a->len == 0)
        return -1;

    for (size_t i = 0; i < a->len; i += a->type_size) {
        if (a->type_cmp(a->val + i, e).eq)
            return i / a->type_size;
    }

    return -1;
}

size_t __cmodules_list_index(list l, raw_string type, size_t type_size, void* e) {
    if (l->len == 0)
        return -1;

    size_t cur = 0;
    list_node node = l->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return cur;
        }
        node = node->next;
        cur += 1;
    }

    return -1;
}

size_t __cmodules_set_index(set l, raw_string type, size_t type_size, void* e) {
    if (l->len == 0)
        return -1;

    size_t cur = 0;
    list_node node = l->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return cur;
        }
        node = node->next;
        cur += 1;
    }

    return -1;
}
