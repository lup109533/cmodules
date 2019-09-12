/** copy(X) **/
string __cmodules_string_copy(string s) {
    string result = new(string);

    result->len = s->len;
    for (size_t i = 0; i < s->len; i++) {
        result->val[i] = s->val[i];
    }

    return result;
}

array  __cmodules_array_copy(array a) {
    array result = alloc(__cmodules_array_struct);

    result->len       = a->len;
    result->type_id   = alloc_array(char, strlen(a->type_id));
    result->type_size = a->type_size;
    strcpy(result->type_id, a->type_id);

    result->type_copy = a->type_copy;
    result->type_del  = a->type_del;

    result->val = alloc_array(byte, adapt_container_length(result->len * result->type_size));
    for (size_t i = 0; i < result->len; i += a->type_size) {
        memcpy(result->val + i, result->type_copy(a->val + i), a->type_size);
    }

    return result;
}

list   __cmodules_list_copy(list l) {
    list result = new(list);

    result->len = l->len;

    if (result->len > 0) {
        list_node source_node = l->head;
        list_node dest_node;

        dest_node = alloc(__cmodules_list_node_struct);
        result->head = dest_node;

        dest_node->type_copy = source_node->type_copy;
        dest_node->type_del  = source_node->type_del;
        dest_node->type_id   = alloc_string(strlen(source_node->type_id));
        dest_node->type_size = source_node->type_size;
        strcpy(dest_node->type_id, source_node->type_id);
        dest_node->val = dest_node->type_copy(source_node->val);

        for (size_t i = 1; i < result->len; i++) {
            dest_node->next = alloc(__cmodules_list_node_struct);
            dest_node   = dest_node->next;
            source_node = source_node->next;

            dest_node->type_copy = source_node->type_copy;
            dest_node->type_del  = source_node->type_del;
            dest_node->type_id   = alloc_string(strlen(source_node->type_id));
            dest_node->type_size = source_node->type_size;
            strcpy(dest_node->type_id, source_node->type_id);
            dest_node->val = dest_node->type_copy(source_node->val);
        }

        dest_node->next = NULL;
    }
    else {
        result->head = NULL;
    }

    return result;
}

fifo __cmodules_fifo_copy(fifo q) {
    fifo result = new(fifo);

    result->len = q->len;

    if (result->len > 0) {
        fifo_node source_node = q->head;
        fifo_node dest_node;
        fifo_node prev_node;

        dest_node = alloc(__cmodules_list_node_struct);
        result->head = dest_node;

        dest_node->prev      = NULL;
        dest_node->type_copy = source_node->type_copy;
        dest_node->type_del  = source_node->type_del;
        dest_node->type_id   = alloc_string(strlen(source_node->type_id));
        dest_node->type_size = source_node->type_size;
        strcpy(dest_node->type_id, source_node->type_id);
        dest_node->val = dest_node->type_copy(source_node->val);

        for (size_t i = 1; i < result->len; i++) {
            dest_node->next = alloc(__cmodules_list_node_struct);
            prev_node   = dest_node;
            dest_node   = dest_node->next;
            source_node = source_node->next;

            dest_node->prev      = prev_node;
            dest_node->type_copy = source_node->type_copy;
            dest_node->type_del  = source_node->type_del;
            dest_node->type_id   = alloc_string(strlen(source_node->type_id));
            dest_node->type_size = source_node->type_size;
            strcpy(dest_node->type_id, source_node->type_id);
            dest_node->val = dest_node->type_copy(source_node->val);
        }
        dest_node->next = NULL;
        result->tail    = dest_node;
    }
    else {
        result->head = NULL;
        result->tail = NULL;
    }

    return result;
}

stack __cmodules_stack_copy(stack s) {
    stack result = new(stack);

    result->len = s->len;

    if (result->len > 0) {
        list_node source_node = s->head;
        list_node dest_node;

        dest_node = alloc(__cmodules_list_node_struct);
        result->head = dest_node;

        dest_node->type_copy = source_node->type_copy;
        dest_node->type_del  = source_node->type_del;
        dest_node->type_id   = alloc_string(strlen(source_node->type_id));
        dest_node->type_size = source_node->type_size;
        strcpy(dest_node->type_id, source_node->type_id);
        dest_node->val = dest_node->type_copy(source_node->val);

        for (size_t i = 1; i < result->len; i++) {
            dest_node->next = alloc(__cmodules_list_node_struct);
            dest_node   = dest_node->next;
            source_node = source_node->next;

            dest_node->type_copy = source_node->type_copy;
            dest_node->type_del  = source_node->type_del;
            dest_node->type_id   = alloc_string(strlen(source_node->type_id));
            dest_node->type_size = source_node->type_size;
            strcpy(dest_node->type_id, source_node->type_id);
            dest_node->val = dest_node->type_copy(source_node->val);
        }

        dest_node->next = NULL;
    }
    else {
        result->head = NULL;
    }

    return result;
}

dict __cmodules_dict_copy(dict d) {
    dict result = alloc(__cmodules_dict_struct);

    result->vals          = alloc_array(dict_node, d->len);
    result->keys          = alloc_array(void*,     d->len);
    result->len           = d->len;
    result->bucket_size   = d->bucket_size;

    result->key_type_id   = alloc_string(strlen(d->key_type_id));
    result->val_type_id   = alloc_string(strlen(d->val_type_id));
    result->key_type_size = d->key_type_size;
    result->val_type_size = d->val_type_size;

    result->key_type_copy = d->key_type_copy;
    result->key_type_del  = d->key_type_del;
    result->val_type_copy = d->val_type_copy;
    result->val_type_del  = d->val_type_del;
    result->hash_key      = d->hash_key;

    strcpy(result->key_type_id, d->key_type_id);
    strcpy(result->val_type_id, d->val_type_id);

    for (size_t i = 0; i < result->len; i++) {
        result->keys[i] = result->key_type_copy(d->keys[i]);
        dict_node dest_node   = result->vals[i];
        dict_node source_node = d->vals[i];

        dest_node->val = result->val_type_copy(source_node->val);
        dest_node->key = result->key_type_copy(source_node->key);

        dest_node->has_collision = source_node->has_collision;
        while (dest_node->has_collision) {
            source_node = source_node->next_for_key;

            dest_node->next_for_key = alloc(__cmodules_dict_node_struct);
            dest_node = dest_node->next_for_key;

            dest_node->val = result->val_type_copy(source_node->val);
            dest_node->key = result->key_type_copy(source_node->key);

            dest_node->has_collision = source_node->has_collision;
        }
        dest_node->next_for_key = NULL;
    }

    return result;
}

set __cmodules_set_copy(set s) {
    set result = new(set);

    result->len = s->len;

    if (result->len > 0) {
        list_node source_node = s->head;
        list_node dest_node;

        dest_node = alloc(__cmodules_list_node_struct);
        result->head = dest_node;

        dest_node->type_copy = source_node->type_copy;
        dest_node->type_del  = source_node->type_del;
        dest_node->type_id   = alloc_string(strlen(source_node->type_id));
        dest_node->type_size = source_node->type_size;
        strcpy(dest_node->type_id, source_node->type_id);
        dest_node->val = dest_node->type_copy(source_node->val);

        for (size_t i = 1; i < result->len; i++) {
            dest_node->next = alloc(__cmodules_list_node_struct);
            dest_node   = dest_node->next;
            source_node = source_node->next;

            dest_node->type_copy = source_node->type_copy;
            dest_node->type_del  = source_node->type_del;
            dest_node->type_id   = alloc_string(strlen(source_node->type_id));
            dest_node->type_size = source_node->type_size;
            strcpy(dest_node->type_id, source_node->type_id);
            dest_node->val = dest_node->type_copy(source_node->val);
        }

        dest_node->next = NULL;
    }
    else {
        result->head = NULL;
    }

    return result;
}

list_node __cmodules_list_node_copy(list_node node) {
    list_node result = alloc(__cmodules_list_node_struct);

    result->type_id   = alloc_array(char, strlen(node->type_id));
    strcpy(result->type_id, node->type_id);
    result->type_size = node->type_size;
    result->type_del  = node->type_del;
    result->type_copy = node->type_copy;
    result->type_cmp  = node->type_cmp;
    result->val = node->type_copy(node->val);
    result->next = NULL;

    return result;
}

fifo_node __cmodules_fifo_node_copy(fifo_node node) {
    fifo_node result = alloc(__cmodules_fifo_node_struct);

    result->type_id   = alloc_array(char, strlen(node->type_id));
    strcpy(result->type_id, node->type_id);
    result->type_size = node->type_size;
    result->type_del  = node->type_del;
    result->type_copy = node->type_copy;
    result->type_cmp  = node->type_cmp;
    result->val = node->type_copy(node->val);
    result->next = NULL;
    result->prev = NULL;

    return result;
}
