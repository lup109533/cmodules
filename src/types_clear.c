void __cmodules_string_clear(string s) {
    memset(s->val, 0, s->mem);
    s->len = 0;
}

void __cmodules_array_clear(array a) {
    memset(a->val, 0, a->mem);
    a->len = 0;
}

void __cmodules_list_clear(list l) {
    list_node node = l->head;
    list_node prev_node;

    while (node != NULL) {
        node->type_del(node->val);
        prev_node = node;
        node = node->next;
        free(prev_node);
    }

    l->head = NULL;
    l->len = 0;
}

void __cmodules_fifo_clear(fifo q) {
    fifo_node node = q->head;
    fifo_node prev_node;

    while (node != NULL) {
        node->type_del(node->val);
        prev_node = node;
        node = node->next;
        free(prev_node);
    }

    q->head = NULL;
    q->tail = NULL;
    q->len = 0;
}

void __cmodules_stack_clear(stack l) {
    list_node node = l->head;
    list_node prev_node;

    while (node != NULL) {
        node->type_del(node->val);
        prev_node = node;
        node = node->next;
        free(prev_node);
    }

    l->head = NULL;
    l->len = 0;
}

void __cmodules_dict_clear(dict d) {
    for (size_t i = 0; i < d->len; i++) {
        d->key_type_del(d->keys[i]);
        d->keys[i] = NULL;
    }
    for (size_t i = 0; i < d->bucket_size; i++) {
        dict_node node = d->vals[i];
        if (node == NULL)
            continue;
        d->key_type_del(node->key);
        d->val_type_del(node->val);
        if (node->has_collision) {
            dict_node base_node = node;
            while (node->has_collision) {
                node = node->next_for_key;
                d->key_type_del(node->key);
                d->val_type_del(node->val);
            }
            node = base_node;
        }
        free(node);
    }
    d->len = 0;
}

void __cmodules_set_clear(set l) {
    list_node node = l->head;
    list_node prev_node;

    while (node != NULL) {
        node->type_del(node->val);
        prev_node = node;
        node = node->next;
        free(prev_node);
    }

    l->head = NULL;
    l->len = 0;
}
