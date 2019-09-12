string __cmodules_string_add (string s, raw_string arg) {
    size_t arg_len = strlen(arg);
    if (arg_len == 0)
        return s;

    if (s->len + arg_len > s->mem) {
        s->mem = adapt_container_length((s->len + arg_len) * sizeof(uchar));
        realloc(s->val, s->mem);
    }

    for (size_t i = 0; i < arg_len;) {
        uchar  cur = utf8_to_code_point(arg + i);
        size_t cln = code_point_size(cur);
        s->val[s->len] = cur;
        s->len += 1;
        i += cln;
    }

    return s;
}

array __cmodules_array_add(array a, size_t arg_size, raw_pointer* arg) {
    if ((a->len + 1) * a->type_size > a->mem) {
        a->mem = adapt_container_length((a->len + 1) * a->type_size);
        realloc(a->val, a->mem);
    }

    for (size_t i = 0; i < arg_size; i++) {
        setval(a, a->len, a->type_copy(arg[i]));
        a->len += 1;
    }

    return a;
}

list __cmodules_list_add(list l, raw_string type, size_t type_size, raw_pointer arg) {
    list_node node;

    if (l->len == 0) {
        l->head = alloc(__cmodules_list_node_struct);
        node = l->head;
    }
    else {
        node = l->head;
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = alloc(__cmodules_list_node_struct);
        node = node->next;
    }

    node->type_id = alloc_array(char, strlen(type)+1);
    strcpy(node->type_id, type);
    node->type_size = type_size;
    node->type_copy = __cmodules_get_copy_function(type);
    node->type_del  = __cmodules_get_del_function(type);
    node->type_cmp  = __cmodules_get_cmp_function(type);
    node->val = node->type_copy(arg);
    node->next = NULL;

    l->len += 1;
    return l;
}

fifo __cmodules_fifo_add(fifo q, raw_string type, size_t type_size, raw_pointer arg) {
    fifo_node node;

    if (q->len == 0) {
        node = alloc(__cmodules_list_node_struct);
        node->next = NULL;
        q->tail = node;
    }
    else {
        node = alloc(__cmodules_list_node_struct);
        node->next = q->head;
        q->head->prev = node;
    }

    node->type_id = alloc_array(char, strlen(type)+1);
    strcpy(node->type_id, type);
    node->type_size = type_size;
    node->type_copy = __cmodules_get_copy_function(type);
    node->type_del  = __cmodules_get_del_function(type);
    node->type_cmp  = __cmodules_get_cmp_function(type);
    node->val = node->type_copy(arg);
    node->prev = NULL;

    q->head = node;
    q->len += 1;
    return q;
}

stack __cmodules_stack_add(stack l, raw_string type, size_t type_size, raw_pointer arg) {
    list_node node;
    list_node next_node;

    if (l->len == 0) {
        l->head = alloc(__cmodules_list_node_struct);
        node = l->head;
        next_node = NULL;
    }
    else {
        node = alloc(__cmodules_list_node_struct);
        next_node = l->head;
    }

    node->type_id = alloc_array(char, strlen(type)+1);
    strcpy(node->type_id, type);
    node->type_size = type_size;
    node->type_copy = __cmodules_get_copy_function(type);
    node->type_del  = __cmodules_get_del_function(type);
    node->type_cmp  = __cmodules_get_cmp_function(type);
    node->val = node->type_copy(arg);
    node->next = next_node;

    l->head = node;
    l->len += 1;
    return l;
}

dict __cmodules_dict_add(dict d, raw_pointer key, raw_pointer val) {
    dict_node node;

    if (__cmodules_dict_has_key(d, key)) {
        __cmodules_dict_setval(d, key, val);
        return d;
    }
    else {
        d->keys[d->len] = d->key_type_copy(key);
        size_t hsh = d->hash_key(key, d->bucket_size);
        if (d->vals[hsh] != NULL) {
            node = d->vals[hsh];
            while (node->has_collision) {
                node = node->next_for_key;
            }
            node->has_collision = true;
            node->next_for_key = alloc(__cmodules_dict_node_struct);
            node = node->next_for_key;
        }
        else {
            d->vals[hsh] = alloc(__cmodules_dict_node_struct);
            node = d->vals[hsh];
        }
    }

    node->val = d->val_type_copy(val);
    node->key = d->key_type_copy(key);
    node->has_collision = false;
    node->next_for_key = NULL;

    d->len += 1;
    return d;
}

set __cmodules_set_add(set l, raw_string type, size_t type_size, raw_pointer arg) {
    if (__cmodules_set_contains(l, type, type_size, arg))
        return l;

    return (set) __cmodules_list_add((list) l, type, type_size, arg);
}
