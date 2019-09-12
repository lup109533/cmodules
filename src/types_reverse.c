string __cmodules_string_reverse(string s) {
    if (s->len == 0)
        return s;

    uchar* rev = alloc_array(uchar, s->len);

    for (size_t i = 0; i < s->len; i++) {
        rev[i] = s->val[i];
    }

    size_t cur = 0;
    for (size_t i = s->len-1; i + 1 > i; i--) {
        s->val[cur] = rev[i];
        cur += 1;
    }

    free(rev);
    return s;
}

array  __cmodules_array_reverse(array a) {
    if (a->len == 0)
        return a;

    byte* rev = alloc_array(byte, a->type_size * a->len);

    for (size_t i = 0; i < a->len; i += a->type_size) {
        memcpy(rev + i, a->val + i, a->type_size);
    }

    size_t cur = 0;
    for (size_t i = a->len-1; i + 1 > i; i -= a->type_size) {
        memcpy(a->val + i, a->val + cur, a->type_size);
        cur += a->type_size;
    }

    free(rev);
    return a;
}

list __cmodules_list_reverse(list l) {
    if (l->len == 0)
        return l;

    list_node* nodes = alloc_array(list_node, l->len);

    list_node node = l->head;
    for (size_t i = 0; i < l->len; i++) {
        nodes[i] = node;
        node = node->next;
    }

    for (size_t i = l->len-1; i != 0; i--) {
        nodes[i]->next = nodes[i-1];
    }
    nodes[0]->next = NULL;
    l->head = nodes[l->len-1];

    free(nodes);
    return l;
}

stack __cmodules_stack_reverse(stack l) {
    if (l->len == 0)
        return l;

    list_node* nodes = alloc_array(list_node, l->len);

    list_node node = l->head;
    for (size_t i = 0; i < l->len; i++) {
        nodes[i] = node;
        node = node->next;
    }

    for (size_t i = l->len-1; i != 0; i--) {
        nodes[i]->next = nodes[i-1];
    }
    nodes[0]->next = NULL;
    l->head = nodes[l->len-1];

    free(nodes);
    return l;
}

fifo __cmodules_fifo_reverse(fifo q) {
    if (q->len == 0)
        return q;

    fifo_node* nodes = alloc_array(list_node, q->len);

    fifo_node node = q->head;
    for (size_t i = 0; i < q->len; i++) {
        nodes[i] = node;
        node = node->next;
    }

    nodes[q->len-1]->prev = NULL;
    for (size_t i = q->len-1; i != 0; i--) {
        nodes[i]->next = nodes[i-1];
        nodes[i-1]->prev = nodes[i];
    }
    nodes[0]->next = NULL;
    q->head = nodes[q->len-1];
    q->tail = nodes[0];

    free(nodes);
    return q;
}

set __cmodules_set_reverse(set l) {
    if (l->len == 0)
        return l;

    list_node* nodes = alloc_array(list_node, l->len);

    list_node node = l->head;
    for (size_t i = 0; i < l->len; i++) {
        nodes[i] = node;
        node = node->next;
    }

    for (size_t i = l->len-1; i != 0; i--) {
        nodes[i]->next = nodes[i-1];
    }
    nodes[0]->next = NULL;
    l->head = nodes[l->len-1];

    free(nodes);
    return l;
}
