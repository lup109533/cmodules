// Pointer
typedef struct {
    raw_pointer*  val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_pointer_array_struct;
typedef __cmodules_pointer_array_struct* pointer_array;

// int16
typedef struct {
    s16*          val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_int16_array_struct;
typedef __cmodules_int16_array_struct* int16_array;
// int32
typedef struct {
    s32*          val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_int32_array_struct;
typedef __cmodules_int32_array_struct* int32_array;
// int64
typedef struct {
    s64*          val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_int64_array_struct;
typedef __cmodules_int64_array_struct* int64_array;

// float32
typedef struct {
    f32*          val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_float32_array_struct;
typedef __cmodules_float32_array_struct* float32_array;
// float64
typedef struct {
    f64*          val;
    size_t        len;
    size_t        mem;
    raw_string    type_id;
    size_t        type_size;
    copy_function type_copy;
    del_function  type_del;
    cmp_function  type_cmp;
} __cmodules_float64_array_struct;
typedef __cmodules_float64_array_struct* float64_array;
