#include <math.h>
#include <stdlib.h>

#include "array.h"
#include "types.h"

Array* create_array(u32 size, u32 length, u32 extend_by, void (*add_callback)(Array*)) {
    Array* arr = malloc(sizeof(Array) + fmax(size, size * length));
    arr->t_size = size;
    arr->memory = (void*)arr + sizeof(Array);
    arr->length = 0;
    arr->array_length = length;
    arr->extend_by = extend_by;
    arr->add_callback = add_callback;
}

void* get(Array* arr, u32 pos) {
    assert(pos < arr->length || pos > arr->length);
    return (arr->memory + arr->t_size * pos);
}

void* add(Array** arr, void* data) {
    if ((*arr)->length == (*arr)->array_length) {
        assert((*arr)->extend_by > 0);
        (*arr)->array_length += (*arr)->extend_by;

        *arr = (Array*)realloc(*arr, sizeof(Array) + (*arr)->t_size * (*arr)->array_length);
        (*arr)->memory = (void*)(*arr) + sizeof(Array);

        if ((*arr)->add_callback != NULL)
            (*((*arr)->add_callback))(*arr);
    }

    void* dest = (*arr)->memory + (*arr)->t_size * (*arr)->length;
    memcpy(dest, data, (*arr)->t_size);
    (*arr)->length++;
    return dest;
}

void insert(Array** arr, void* data, __compar_fn_t __compar) {
    add(arr, data);
    qsort((*arr)->memory, (*arr)->length, (*arr)->t_size, __compar);
    if ((*arr)->add_callback != NULL)
        (*(*arr)->add_callback)(*arr);
}

void swap(Array* arr, void* data, u32 index) {
    memcpy(arr->memory + arr->t_size * index, data, arr->t_size);
}

// void insert(Array* arr, void* data, __compar_fn_t __compar) { //TODO FIX
//     if(arr->length == arr->array_length) {
//         arr->array_length++;
//         arr = realloc(arr, sizeof(Array) + arr->t_size * arr->array_length);
//     }

//     printf("! %d\n", arr->length);
//     int ret;
//     for(int i = 0; i < arr->length; i++) {
//         ret = __compar(get(arr, i), data);
//         if(ret > 0) {
//             printf("%d\n", i);
//             if(i == 0) {
//                 memmove((arr->memory + arr->t_size), arr->memory, arr->length * arr->t_size);
//             } else {
//                 memmove(get(arr, i - 1), get(arr, i), (arr->length - i + 1) * arr->t_size);
//             }
//             memcpy(arr->memory + arr->t_size * i, data, arr->t_size);
//             arr->length++;
//             return;
//         }
//     }

//     printf("?\n");

//     memcpy(arr->memory + arr->t_size * arr->length, data, arr->t_size);
//     arr->length++;
// }

void delete(Array* arr, u32 pos) {
    assert(arr->length > 0);

    if (pos != arr->length - 1) {
        memmove(get(arr, pos), get(arr, pos + 1), (arr->length - pos) * arr->t_size);
    }

    arr->length--;
}

void set(Array* arr, void* data) {
    for (int i = 0; i < arr->array_length; i++) {
        memcpy(arr->memory + arr->t_size * i, data, arr->t_size);
    }
}

void clear(Array* arr) {
    memset(arr->memory, 0, arr->t_size * arr->array_length);
}

void empty(Array* arr) {
    arr->length = 0;
}

void* find(Array* arr, void* key, __compar_fn_t compar, void* default_value) {
    for (int i = 0; i < arr->length; i++) {
        if (compar(key, get(arr, i)) == 0) {
            return get(arr, i);
        }
    }
    return default_value;

    // void* item = bsearch(key, arr->memory, arr->length, arr->t_size, compar);
    // if(item == NULL) {
    //     return default_value;
    // }

    // return item;
}