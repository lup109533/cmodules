#include <stdlib.h>
#include <stdarg.h>

#include "copy.c"
#include "del.c"
#include "hash.c"
#include "cmp.c"
#include "types_new.c"
#include "types_del.c"
#include "types_copy.c"
#include "types_add.c"
#include "types_rem.c"
#include "types_clear.c"
#include "types_contains.c"
#include "types_index.c"
#include "types_join.c"
#include "types_slice.c"
#include "types_reverse.c"
#include "types_sort.c"
#include "types_map.c"
#include "types_filter.c"
#include "types_reduce.c"
#include "types_getval.c"
#include "types_setval.c"

s32 __cmodules_s32_mcd(s32 x, s32 y) {
    s32 t;
    while (y != 0) {
        t = y;
        y = x % y;
        x = t;
    }
    return x;
}

s64 __cmodules_s64_mcd(s64 x, s64 y) {
    s64 t;
    while (y != 0) {
        t = y;
        y = x % y;
        x = t;
    }
    return x;
}

u32 __cmodules_u32_mcd(u32 x, u32 y) {
    u32 t;
    while (y != 0) {
        t = y;
        y = x % y;
        x = t;
    }
    return x;
}

u64 __cmodules_u64_mcd(u64 x, u64 y) {
    u64 t;
    while (y != 0) {
        t = y;
        y = x % y;
        x = t;
    }
    return x;
}

void* __cmodules_stack_pop(stack s) {
    if (s->len == 0)
        return NULL;

    list_node node = s->head;
    raw_pointer result = node->val;

    s->head = node->next;
    s->len -= 1;
    free(node);

    return result;
}

void* __cmodules_fifo_pop(fifo q) {
    if (q->len == 0)
        return NULL;

    fifo_node node = q->tail;
    raw_pointer result = node->val;
    if (q->len == 1) {
        q->head = NULL;
        q->tail = NULL;
    }
    else {
        node->prev->next = NULL;
        q->tail = node->prev;
    }
    q->len -= 1;
    free(node);

    return result;
}

list_node __cmodules_stack_pop_node(stack s) {
    if (s->len == 0)
        return NULL;

    list_node node = s->head;

    s->head = node->next;
    s->len -= 1;

    return node;
}

fifo_node __cmodules_fifo_pop_node(fifo q) {
    if (q->len == 0)
        return NULL;

    fifo_node node   = q->tail;
    if (q->len == 1) {
        q->head = NULL;
        q->tail = NULL;
    }
    else {
        node->prev->next = NULL;
    }
    q->len -= 1;

    return node;
}

dict_node __cmodules_dict_get_node(dict d, void* key) {
    size_t hsh = d->hash_key(key, d->bucket_size);

    dict_node node = d->vals[hsh];
    while (node->has_collision) {
        if (d->key_type_cmp(key, node->key).eq) {
            break;
        }
        node = node->next_for_key;
    }

    return node;
}

bool __cmodules_dict_has_key(dict d, void* key) {
    for (size_t i = 0; i < d->len; i++) {
        if (d->key_type_cmp(d->keys[i], key).eq) {
            return true;
        }
    }

    return false;
}

bool __cmodules_set_has_node(set s, list_node n) {
    list_node node = s->head;

    while (node != NULL) {
        if (strcmp(node->type_id, n->type_id)) {
            if (node->type_cmp(node->val, n->val).eq)
                return true;
        }
        node = node->next;
    }

    return false;
}

size_t __cmodules_get_type_size(raw_string s) {
    if (strcmp(s, "s8") == 0 || strcmp(s, "u8") == 0 || strcmp(s, "byte") == 0) {
       return sizeof(s8);
    }
    elif (strcmp(s, "s16") == 0 || strcmp(s, "u16") == 0) {
       return sizeof(s16);
    }
    elif (strcmp(s, "s32") == 0 || strcmp(s, "u32") == 0 || strcmp(s, "f32") == 0) {
       return sizeof(s32);
    }
    elif (strcmp(s, "s64") == 0 || strcmp(s, "u64") == 0 || strcmp(s, "f64") == 0) {
       return sizeof(s64);
    }
    else {
        return sizeof(intptr_t);
    }
}
