// Insertion sort algorithms
string __cmodules_string_insertion_sort(string s) {
    if (s->len < 2)
        return s;

    uchar* sorted = alloc_array(uchar, s->len);
    sorted[0] = s->val[0];
    for (size_t i = 1; i < s->len; i++) {
        sorted[i] = s->val[i];
        for (size_t j = i; j > 0 && sorted[j-1] > sorted[j]; j--) {
            uchar tmp = sorted[j];
            sorted[j] = sorted[j-1];
            sorted[j-1] = tmp;
        }
    }

    memcpy(s->val, sorted, s->len * sizeof(uchar));
    free(sorted);
    return s;
}

array __cmodules_array_insertion_sort(array a) {
    if (a->len < 2)
        return a;

    array tmp_array = __cmodules_array_copy(a);
    __cmodules_array_setval(tmp_array, 0, __cmodules_array_getval(a, 0));
    for (size_t i = 1; i < a->len; i++) {
        __cmodules_array_setval(tmp_array, i, __cmodules_array_getval(a, i));
        for (size_t j = i;
                    j > 0 &&
                    tmp_array->type_cmp(__cmodules_array_getval(tmp_array, j-1), __cmodules_array_getval(tmp_array, j)).gt;
                    j--) {
            raw_pointer tmp = __cmodules_array_getval(tmp_array, j);
            __cmodules_array_setval(a, j,   __cmodules_array_getval(tmp_array, j-1));
            __cmodules_array_setval(a, j-1, tmp);
        }
    }

    free(a->val);
    a->val = tmp_array->val;
    __cmodules_array_del(tmp_array);
    return a;
}

// Quicksort
// string
uchar __cmodules_string_pivot(string s, size_t lo, size_t hi) {
    size_t mid = (lo + hi) / 2;
    if (s->val[mid] < s->val[lo]) {
        uchar tmp = s->val[mid];
        s->val[mid] = s->val[lo];
        s->val[lo] = tmp;
    }
    if (s->val[hi] < s->val[lo]) {
        uchar tmp = s->val[hi];
        s->val[hi] = s->val[lo];
        s->val[lo] = tmp;
    }
    if (s->val[mid] < s->val[hi]) {
        uchar tmp = s->val[mid];
        s->val[mid] = s->val[hi];
        s->val[hi] = tmp;
    }
    return s->val[hi];
}
void __cmodules_string_partition(string s, uchar p, size_t lo, size_t hi, size_t* left, size_t* right) {
    size_t i = lo - 1;
    size_t j = hi + 1;
    while (true) {
        do {
            i += 1;
        } while (s->val[i] < p);
        do {
            j -= 1;
        } while (s->val[j] > p);

        if (i >= j) {
            *left  = i;
            *right = j;
            return;
        }

        uchar tmp = s->val[i];
        s->val[i] = s->val[j];
        s->val[j] = tmp;
    }
}
string __cmodules_string_quicksort(string s, size_t lo, size_t hi) {
    if (lo < hi) {
        uchar p = __cmodules_string_pivot(s, lo, hi);
        size_t left;
        size_t right;
        __cmodules_string_partition(s, p, lo, hi, &left, &right);
        __cmodules_string_quicksort(s, lo, left-1);
        __cmodules_string_quicksort(s, right+1, hi);
    }
    return s;
}

// array
raw_pointer __cmodules_array_pivot(array a, size_t lo, size_t hi) {
    size_t mid = (lo + hi) / 2;
    raw_pointer lo_val  = __cmodules_array_getval(a, lo);
    raw_pointer mid_val = __cmodules_array_getval(a, mid);
    raw_pointer hi_val  = __cmodules_array_getval(a, hi);
    bool switch_happened = false;
    if (a->type_cmp(mid_val, lo_val).lt) {
        raw_pointer tmp = mid_val;
        mid_val = lo_val;
        lo_val  = tmp;
        switch_happened = true;
    }
    if (a->type_cmp(hi_val, lo_val).lt) {
        raw_pointer tmp = hi_val;
        hi_val = lo_val;
        lo_val = tmp;
        switch_happened = true;
    }
    if (a->type_cmp(mid_val, hi_val).lt) {
        raw_pointer tmp = mid_val;
        mid_val = hi_val;
        hi_val = tmp;
        switch_happened = true;
    }
    if (switch_happened) {
        __cmodules_array_setval(a, lo,  lo_val);
        __cmodules_array_setval(a, mid, mid_val);
        __cmodules_array_setval(a, hi,  hi_val);
    }
    return hi_val;
}
void __cmodules_array_partition(array a, raw_pointer p, size_t lo, size_t hi, size_t* left, size_t* right) {
    size_t i = lo - 1;
    size_t j = hi + 1;
    raw_pointer i_val = __cmodules_array_getval(a, i);
    raw_pointer j_val = __cmodules_array_getval(a, j);
    while (true) {
        do {
            i += 1;
            i_val = __cmodules_array_getval(a, i);
        } while (a->type_cmp(i_val, p).lt);
        do {
            j -= 1;
            j_val = __cmodules_array_getval(a, j);
        } while (a->type_cmp(j_val, p).gt);

        if (i >= j) {
            *left  = i;
            *right = j;
            return;
        }

        raw_pointer tmp = i_val;
        i_val = j_val;
        j_val = tmp;
        __cmodules_array_setval(a, i, i_val);
        __cmodules_array_setval(a, j, j_val);
    }
}
array __cmodules_array_quicksort(array a, size_t lo, size_t hi) {
    if (lo < hi) {
        raw_pointer p = __cmodules_array_pivot(a, lo, hi);
        size_t left;
        size_t right;
        __cmodules_array_partition(a, p, lo, hi, &left, &right);
        __cmodules_array_quicksort(a, lo, left-1);
        __cmodules_array_quicksort(a, right+1, hi);
    }
    return a;
}

// Mergesort
list_node* __cmodules_list_mergesort_merge_by_type(list_node* nodes, size_t beg, size_t mid, size_t end) {
    list_node* result = alloc_array(list_node, end - beg);
    size_t l = beg;
    size_t r = mid;
    size_t cur = 0;

    while (l < mid && r < end) {
        int type_cmp = strcmp(nodes[l]->type_id, nodes[r]->type_id);
        if (type_cmp < 0) {
            result[cur] = nodes[l];
            l += 1;
        }
        else {
            result[cur] = nodes[r];
            r += 1;
        }
        cur += 1;
    }

    while (l < mid) {
        result[cur] = nodes[l];
        l += 1;
        cur += 1;
    }
    while (r < end) {
        result[cur] = nodes[r];
        r += 1;
        cur += 1;
    }

    for (size_t i = 0; i < cur; i++) {
        nodes[beg+i] = result[i];
    }

    free(result);
    return nodes;
}

list_node* __cmodules_list_mergesort_merge_by_value(list_node* nodes, size_t beg, size_t mid, size_t end) {
    list_node* result = alloc_array(list_node, end - beg);
    size_t l = beg;
    size_t r = mid;
    size_t cur = 0;

    while (l < mid && r < end) {
        int type_cmp = strcmp(nodes[l]->type_id, nodes[r]->type_id);
        if (type_cmp != 0) {
            result[cur] = nodes[l];
            l += 1;
        }
        else {
            cmp_result value_cmp = nodes[l]->type_cmp(nodes[l]->val, nodes[r]->val);
            if (value_cmp.le) {
                result[cur] = nodes[l];
                l += 1;
            }
            else {
                result[cur] = nodes[r];
                r += 1;
            }
        }
        cur += 1;
    }

    while (l < mid) {
        result[cur] = nodes[l];
        l += 1;
        cur += 1;
    }
    while (r < end) {
        result[cur] = nodes[r];
        r += 1;
        cur += 1;
    }

    for (size_t i = 0; i < cur; i++) {
        nodes[beg+i] = result[i];
    }

    free(result);
    return nodes;
}

list_node* __cmodules_list_mergesort_split(list_node* nodes, size_t beg, size_t end, bool sort_by_type) {
    if (end - beg <= 1)
        return nodes;

    size_t mid = (beg + end) / 2;

    __cmodules_list_mergesort_split(nodes, beg, mid, sort_by_type);   // Left
    __cmodules_list_mergesort_split(nodes, mid, end, sort_by_type); // Right

    if (sort_by_type)
        return __cmodules_list_mergesort_merge_by_type(nodes, beg, mid, end);
    else
        return __cmodules_list_mergesort_merge_by_value(nodes, beg, mid, end);
}

list __cmodules_list_mergesort(list l) {
    if (l->len <= 1)
        return l;

    list_node* nodes = alloc_array(list_node, l->len);
    size_t i = 0;
    for (list_node node = l->head; node != NULL; node = node->next) {
        nodes[i] = node;
        i += 1;
    }

    nodes = __cmodules_list_mergesort_split(nodes, 0, l->len, true);  // Sort by type
    nodes = __cmodules_list_mergesort_split(nodes, 0, l->len, false); // Sort by value

    for (i = 0; i < l->len - 1; i++) {
        nodes[i]->next = nodes[i+1];
    }
    nodes[i]->next = NULL;
    l->head = nodes[0];
    free(nodes);

    return l;
}

// Discriminate sort algorithm
string __cmodules_string_sort(string s) {
    if (s->len <= 7)
        return __cmodules_string_insertion_sort(s);
    else
        return __cmodules_string_quicksort(s, 0, s->len-1);
}

array __cmodules_array_sort(array a) {
    if (a->len <= 7)
        return __cmodules_array_insertion_sort(a);
    else
        return __cmodules_array_quicksort(a, 0, a->len-1);
}

list __cmodules_list_sort(list l) {
    return __cmodules_list_mergesort(l);
}

set __cmodules_set_sort(set s) {
    return (set) __cmodules_list_mergesort((list) s);
}
