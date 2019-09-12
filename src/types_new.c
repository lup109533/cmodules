/** new(X, ...) **/
#include "utf8.c"
string __cmodules_string_new (size_t argnum, ...) {
    string result = alloc(__cmodules_string_struct);

    if (argnum > 1) {
        va_list args;
        va_start(args, argnum);
        char*   arg = va_arg(args, char*);
        size_t  len = strlen(arg);
        uchar*  val = alloc_array(uchar, adapt_container_length(len+1));
        for (size_t i = 0; i < len; i++) {
            val[i] = utf8_to_code_point(arg);
            arg += code_point_size(val[i]);
        }
        va_end(args);

        result->val = val;
        result->len = len;
        result->mem = adapt_container_length(len+1);
    }
    else {
        result->val = alloc_array(char, default_container_length);
        result->len = 0;
        result->mem = default_container_length;
    }

    return result;
}

array __cmodules_array_new(const raw_string type, size_t type_size, size_t length, size_t init_length, void* init_const) {
    array result = alloc(__cmodules_array_struct);

    result->mem       = adapt_container_length(type_size*length);
    result->val       = alloc_array(byte, result->mem);
    result->len       = length;
    result->type_id   = alloc_array(char, strlen(type));
    result->type_size = type_size;
    result->type_copy = __cmodules_get_copy_function(type);
    result->type_del  = __cmodules_get_del_function(type);
    result->type_cmp  = __cmodules_get_cmp_function(type);
    strcpy(result->type_id, type);

    if (init_length > 0) {
        void* result_alias = result->val;
        void* initializer  = init_const;

        for (size_t i = 0; i < init_length; i++) {
            memcpy(result_alias, initializer, type_size); // read value from initializer according to type size
            initializer  += type_size;
            result_alias += type_size;
        }
    }

    return result;
}

list __cmodules_list_new() {
    list result = alloc(__cmodules_list_struct);

    result->head = NULL;
    result->len  = 0;

    return result;
}

stack __cmodules_stack_new() {
    stack result = alloc(__cmodules_stack_struct);

    result->head = NULL;
    result->len  = 0;

    return result;
}

fifo __cmodules_fifo_new() {
    fifo result = alloc(__cmodules_fifo_struct);

    result->head = NULL;
    result->tail = NULL;
    result->len  = 0;

    return result;
}

dict __cmodules_dict_new(const raw_string key_type, size_t key_type_size, const raw_string val_type, size_t val_type_size) {
    dict result = alloc(__cmodules_dict_struct);

    result->bucket_size = default_container_length / sizeof(dict_node);
    result->len         = 0;
    result->vals        = alloc_array(dict_node, result->bucket_size);
    result->keys        = alloc_array(u8, result->bucket_size * key_type_size);

//    for (size_t i = 0; i < result->bucket_size; i++) {
//        result->vals[i] = alloc(__cmodules_dict_node_struct);
//        result->vals[i]->val = NULL;
//        result->vals[i]->key = NULL;
//        result->vals[i]->has_collision = false;
//        result->vals[i]->next_for_key  = NULL;
//    }

    size_t string_len = strlen(key_type);
    result->key_type_id = alloc_array(char, string_len+1);
    strcpy(result->key_type_id, key_type);

    string_len = strlen(val_type);
    result->val_type_id = alloc_array(char, string_len+1);
    strcpy(result->val_type_id, val_type);

    result->key_type_size = key_type_size;
    result->val_type_size = val_type_size;

    result->key_type_copy = __cmodules_get_copy_function(key_type);
    result->key_type_del  = __cmodules_get_del_function(key_type);
    result->key_type_cmp  = __cmodules_get_cmp_function(key_type);
    result->val_type_copy = __cmodules_get_copy_function(val_type);
    result->val_type_del  = __cmodules_get_del_function(val_type);
    result->val_type_cmp  = __cmodules_get_cmp_function(val_type);

    result->hash_key = __cmodules_get_hash_function(key_type);

    return result;
}

set __cmodules_set_new() {
    set result = alloc(__cmodules_set_struct);

    result->head = NULL;
    result->len  = 0;

    return result;
}
