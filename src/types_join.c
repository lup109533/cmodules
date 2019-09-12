string __cmodules_string_join(string s1, string s2) {
    if (s1->len + s2->len > s1->mem - 1) {
        s1->mem = adapt_container_length(s1->len + s2->len + 1);
        realloc(s1->val, s1->mem);
    }

    size_t cur = s1->len;
    for (size_t i = 0; i < s2->len; i++) {
        s1->val[cur + i] = s2->val[i];
    }

    s1->len += s2->len;

    return s1;
}

array __cmodules_array_join(array a1, array a2) {
    if (strcmp(a1->type_id, a2->type_id) != 0) {
        return a1;
    }

    if ((a1->len + a2->len) * a1->type_size > a1->mem) {
        a1->mem = adapt_container_length((a1->len + a2->len) * a1->type_size);
        realloc(a1->val, a1->mem);
    }

    size_t cur = a1->len * a1->type_size;
    for (size_t i = 0; i < a2->len; i++) {
        a1->val[cur + i*a1->type_size] = a2->val[i*a2->type_size];
    }

    a1->len += a2->len;

    return a1;
}

list __cmodules_list_join(list l1, list l2) {
    if (l2->len == 0)
        return l1;

    list_node dest_node;
    list_node source_node;
    if (l1->len == 0) {
        l1->head = __cmodules_list_node_copy(l2->head);
        dest_node = l1->head;
        source_node = l2->head->next;
    }
    else {
        dest_node = l1->head;
        while (dest_node->next != NULL) {
            dest_node = dest_node->next;
        }
        source_node = l2->head;
    }

    while (source_node != NULL) {
        dest_node->next = __cmodules_list_node_copy(source_node);
        dest_node   = dest_node->next;
        source_node = source_node->next;
    }

    l1->len += l2->len;

    return l1;
}

dict __cmodules_dict_join(dict d1, dict d2) {
    if (strcmp(d1->key_type_id, d2->key_type_id) != 0 ||
        strcmp(d1->val_type_id, d2->val_type_id) != 0)
        return d1;

    for (size_t i = 0; i < d2->len; i++) {
        dict_node node = d2->vals[i];
        __cmodules_dict_add(d1, node->key, node->val);
    }

    return d1;
}

set __cmodules_set_join(set s1, set s2) {
    if (s2->len == 0)
        return s1;

    if (s1->len == 0) {
        __cmodules_list_join((list) s1, (list) s2);
        return s1;
    }

    list_node dest_node = s1->head;
    while (dest_node->next != NULL) {
        dest_node = dest_node->next;
    }
    list_node source_node = s2->head;

    while (source_node != NULL) {
        if (__cmodules_set_has_node(s1, source_node) == false) {
            dest_node->next = __cmodules_list_node_copy(source_node);
            dest_node   = dest_node->next;
            source_node = source_node->next;
            s1->len += 1;
        }
    }

    return s1;
}
