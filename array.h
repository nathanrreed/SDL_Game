#include "types.h"

#ifndef __compar_fn_t 
typedef int (*__compar_fn_t) (const void *, const void *);
#endif

typedef struct Array {
    void* memory;
    u32 t_size;
    u32 length;
    u32 array_length;
    u32 extend_by;
    void (*add_callback)(struct Array*);
} Array;

Array* create_array(u32 size, u32 length, u32 extend_by, void (*add_callback)(Array*));

void* get(Array* arr, u32 pos);

void* add(Array** arr, void* data);

void insert(Array** arr, void* data, __compar_fn_t __compar);

void delete(Array* arr, u32 pos);

void clear(Array* arr);

void empty(Array* arr);

void* find(Array* arr, void* key, __compar_fn_t compar, void* default_value);

#pragma once