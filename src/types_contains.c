bool __cmodules_string_contains(string s, string e) {
    if (s->len < e->len)
        return false;

    size_t max_index = s->len - e->len;

    for (size_t i = 0; i <= max_index; i++) {
        if (memcmp(s->val + i, e->val, e->len * sizeof(uchar)) == 0)
            return true;
    }

    return false;
}

bool __cmodules_array_contains(array a, raw_pointer e) {
    if (a->len == 0)
        return false;

    for (size_t i = 0; i < a->len; i += a->type_size) {
        if (a->type_cmp(a->val + i, e).eq)
            return true;
    }

    return false;
}

bool __cmodules_list_contains(list l, raw_string type, size_t type_size, raw_pointer e) {
    if (l->len == 0)
        return false;

    list_node node = l->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return true;
        }
        node = node->next;
    }

    return false;
}

bool __cmodules_fifo_contains(fifo q, raw_string type, size_t type_size, raw_pointer e) {
    if (q->len == 0)
        return false;

    fifo_node node = q->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return true;
        }
        node = node->next;
    }

    return false;
}

bool __cmodules_stack_contains(stack s, raw_string type, size_t type_size, raw_pointer e) {
    if (s->len == 0)
        return false;

    list_node node = s->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return true;
        }
        node = node->next;
    }

    return false;
}

bool __cmodules_dict_contains(dict d, raw_pointer e) {
    if (d->len == 0)
        return false;

    for (size_t i = 0; i < d->len; i++) {
        size_t hsh = d->hash_key(d->keys[i], d->bucket_size);
        dict_node node = d->vals[hsh];
        if (d->val_type_cmp(node->val, e).eq)
            return true;
        while (node->has_collision) {
            node = node->next_for_key;
            if (d->val_type_cmp(node->val, e).eq)
                return true;
        }
    }

    return false;
}

bool __cmodules_set_contains(set s, raw_string type, size_t type_size, raw_pointer e) {
    if (s->len == 0)
        return false;

    list_node node = s->head;
    while (node != NULL) {
        if (strcmp(node->type_id, type) == 0) {
            if (node->type_cmp(node->val, e).eq)
                return true;
        }
        node = node->next;
    }

    return false;
}
