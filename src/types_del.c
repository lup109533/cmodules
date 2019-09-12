/** del(X) **/
void __cmodules_string_del(string s) {
    free(s->val);
    free(s);
}

void __cmodules_array_del(array a) {
    if (a->type_size >= sizeof(void*)) {
        for (size_t i = 0; i < a->len; i += a->type_size) {
            a->type_del(a->val + i);
        }
    }

    free(a->val);
    free(a);
}

void __cmodules_list_del(list l) {
    list_node node = l->head;

    while (node != NULL) {
        list_node next_node = node->next;
        node->type_del(node->val);
        free(node);
        node = next_node;
    }
    free(l);
}

void __cmodules_stack_del(stack l) {
    list_node node = l->head;

    while (node != NULL) {
        list_node next_node = node->next;
        node->type_del(node->val);
        free(node);
        node = next_node;
    }
    free(l);
}

void __cmodules_fifo_del(fifo q) {
    fifo_node node = q->head;

    while (node != NULL) {
        fifo_node next_node = node->next;
        node->type_del(node->val);
        free(node);
        node = next_node;
    }
    free(q);
}

void __cmodules_dict_del(dict d) {
    for (size_t i = 0; i < d->len; i++) {
        dict_node node = __cmodules_dict_get_node(d, d->keys[i]);
        if (node->has_collision) {
            dict_node base_node = node;
            node = node->next_for_key;
            while (node->has_collision) {
                d->key_type_del(node->key);
                d->val_type_del(node->val);
                dict_node next_node = node->next_for_key;
                free(node);
                node = next_node;
            }
            node = base_node;
        }
        d->key_type_del(node->key);
        d->val_type_del(node->val);
        d->key_type_del(d->keys[i]);
        free(node);
    }

    free(d->key_type_id);
    free(d->val_type_id);
    free(d->keys);
    free(d->vals);
    free(d);
}

void __cmodules_set_del(set s) {
    list_node node = s->head;

    while (node != NULL) {
        list_node next_node = node->next;
        node->type_del(node->val);
        free(node);
        node = next_node;
    }
    free(s);
}

void __cmodules_list_node_del(list_node node) {
    free(node->type_id);
    node->type_del(node->val);
    free(node);
}

void __cmodules_fifo_node_del(fifo_node node) {
    free(node->type_id);
    node->type_del(node->val);
    free(node);
}
