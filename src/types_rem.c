void __cmodules_string_rem(string s, size_t pos) {
    if (pos > s->len-1)
        pos = s->len-1;

    if (pos == s->mem-1) {
        s->val[pos] = 0;
        s->len -= 1;
        return;
    }

    memcpy(s->val + pos, s->val + pos + 1, (s->len - pos) * sizeof(uchar));
    s->len -= 1;
}

void __cmodules_array_rem(array a, size_t pos) {
    if (pos > a->len-1)
        pos = a->len-1;

    pos *= a->type_size;
    a->type_del(a->val + pos);
    memcpy(a->val + pos, a->val + pos + 1, (a->len - pos));

    a->len -= 1;
}

void __cmodules_list_rem(list l, size_t pos) {
    if (pos > l->len-1)
        pos = l->len-1;

    list_node node;
    list_node prev_node;
    if (pos == 0) {
        node = l->head;
        l->head = node->next;
    }
    else {
        node = l->head->next;
        prev_node = l->head;
        for (size_t i = 1; i < pos; i++) {
            prev_node = node;
            node = node->next;
        }
        prev_node->next = node->next;
    }

    node->type_del(node->val);
    free(node);
    l->len -= 1;
}

void __cmodules_dict_rem(dict d, void* key) {
    size_t hsh = d->hash_key(key, d->bucket_size);
    dict_node node = d->vals[hsh];

    if (d->key_type_cmp(node->key, key).eq) {
        d->key_type_del(node->key);
        d->val_type_del(node->val);
        if (node->has_collision) {
            d->vals[hsh] = node->next_for_key;
        }
        else {
            d->vals[hsh] = NULL;
        }
        free(node);
        d->len -= 1;
    }
    else {
        dict_node prev_node = NULL;
        dict_node curr_node = d->vals[hsh];
        while (d->key_type_cmp(node->key, key).ne && node->has_collision) {
            prev_node = curr_node;
            curr_node = curr_node->next_for_key;
        }
        if (d->key_type_cmp(node->key, key).eq) {
            d->val_type_del(curr_node->val);
            d->key_type_del(curr_node->key);
            prev_node->next_for_key = curr_node->next_for_key;
            free(curr_node);
        }
    }
}

void __cmodules_set_rem(set l, size_t pos) {
    if (pos > l->len-1)
        pos = l->len-1;

    list_node node;
    list_node prev_node;
    if (pos == 0) {
        node = l->head;
        l->head = node->next;
    }
    else {
        node = l->head->next;
        prev_node = l->head;
        for (size_t i = 1; i < pos; i++) {
            prev_node = node;
            node = node->next;
        }
        prev_node->next = node->next;
    }

    node->type_del(node->val);
    free(node);
    l->len -= 1;
}
