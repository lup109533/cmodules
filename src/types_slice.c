string __cmodules_string_slice(string s, size_t p1, size_t p2) {
    string result = new(string);

    if (s->len == 0)
        return result;

    if (p1 > s->len-1)
        p1 = s->len-1;

    if (p2 > s->len-1)
        p2 = s->len-1;

    bool reversed = false;
    if (p1 > p2) {
        size_t tmp = p1;
        p1 = p2;
        p2 = tmp;
        reversed = true;
    }
    result->len = p2 - p1 + 1;

    if (!reversed) {
        size_t cur = 0;
        for (size_t i = p1; i <= p2; i++) {
            result->val[cur] = s->val[i];
            cur += 1;
        }
    }
    else {
        size_t cur = 0;
        for (size_t i = p2; i >= p1 && i < i + 1; i--) {
            result->val[cur] = s->val[i];
            cur += 1;
        }
    }

    return result;
}

array  __cmodules_array_slice (array a, size_t p1, size_t p2) {
    array result = __cmodules_array_new(a->type_id, a->type_size, 0, 0, NULL);

    if (a->len == 0)
        return result;

    if (p1 > a->len)
        p1 = a->len-1;

    if (p2 > a->len)
        p2 = a->len-1;

    bool reversed = false;
    if (p1 > p2) {
        size_t tmp = p1;
        p1 = p2;
        p2 = tmp;
        reversed = true;
    }
    result->len = p2 - p1 + 1;

    if (!reversed) {
        size_t cur = 0;
        for (size_t i = p1 * a->type_size; i <= p2 * a->type_size; i += a->type_size) {
            memcpy(result->val + cur, a->type_copy(a->val + i), a->type_size);
            cur += a->type_size;
        }
    }
    else {
        size_t cur = 0;
        for (size_t i = p2 * a->type_size; i >= p1 * a->type_size && (i - 1) < i; i -= a->type_size) {
            memcpy(result->val + cur, a->type_copy(a->val + i), a->type_size);
            cur += a->type_size;
        }
    }

    return result;
}

list __cmodules_list_slice(list l, size_t p1, size_t p2) {
    list result = __cmodules_list_new();

    if (l->len == 0)
        return result;

    if (p1 > l->len)
        p1 = l->len-1;

    if (p2 > l->len)
        p2 = l->len-1;

    bool reversed = false;
    if (p1 > p2) {
        size_t tmp = p1;
        p1 = p2;
        p2 = tmp;
        reversed = true;
    }
    result->len = p2 - p1 + 1;

    size_t    cur = 0;
    list_node source_node = l->head;

    while (cur < p1) {
        source_node = source_node->next;
        cur += 1;
    }

    result->head = __cmodules_list_node_copy(source_node);
    cur += 1;
    list_node dest_node = result->head;
    while (cur <= p2) {
        source_node     = source_node->next;
        dest_node->next = __cmodules_list_node_copy(source_node);
        dest_node       = dest_node->next;
        cur += 1;
    }

    if (reversed) {
        list_node nodes[result->len];
        list_node node = result->head;

        cur = 0;
        while (node != NULL) {
            nodes[cur] = node;
            node = node->next;
        }

        node = l->head->next;
        l->head = nodes[result->len-1];
        for (size_t i = result->len-2; i > 0 && (i-1) < i; i--) {
            node->next = nodes[i];
            node = node->next;
        }
    }

    return result;
}

set __cmodules_set_slice(set l, size_t p1, size_t p2) {
    set result = __cmodules_set_new();

    if (l->len == 0)
        return result;

    if (p1 > l->len)
        p1 = l->len-1;

    if (p2 > l->len)
        p2 = l->len-1;

    bool reversed = false;
    if (p1 > p2) {
        size_t tmp = p1;
        p1 = p2;
        p2 = tmp;
        reversed = true;
    }
    result->len = p2 - p1 + 1;

    size_t    cur = 0;
    list_node source_node = l->head;

    while (cur < p1) {
        source_node = source_node->next;
        cur += 1;
    }

    result->head = __cmodules_list_node_copy(source_node);
    cur += 1;
    list_node dest_node = result->head;
    while (cur <= p2) {
        source_node     = source_node->next;
        dest_node->next = __cmodules_list_node_copy(source_node);
        dest_node       = dest_node->next;
        cur += 1;
    }

    if (reversed) {
        list_node nodes[result->len];
        list_node node = result->head;

        cur = 0;
        while (node != NULL) {
            nodes[cur] = node;
            node = node->next;
        }

        node = l->head->next;
        l->head = nodes[result->len-1];
        for (size_t i = result->len-2; i > 0 && (i-1) < i; i--) {
            node->next = nodes[i];
            node = node->next;
        }
    }

    return result;
}
