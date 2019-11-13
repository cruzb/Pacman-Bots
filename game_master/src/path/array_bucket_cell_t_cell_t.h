#ifndef ARRAY_BUCKET_CELL_T_CELL_T_H
#define ARRAY_BUCKET_CELL_T_CELL_T_H

#include <inttypes.h>
#include <stdlib.h>
//#define bucket_cell_t_cell_t uint32_t

typedef struct array_bucket_cell_t_cell_t {
    size_t num_els;
    size_t capacity;
    bucket_cell_t_cell_t* els;
} array_bucket_cell_t_cell_t;

void array_bucket_cell_t_cell_t_init(array_bucket_cell_t_cell_t* array, size_t capacity);
void array_bucket_cell_t_cell_t_destroy(array_bucket_cell_t_cell_t* array);
int array_bucket_cell_t_cell_t_empty(array_bucket_cell_t_cell_t* array);
int array_bucket_cell_t_cell_t_full(array_bucket_cell_t_cell_t* array);
int array_bucket_cell_t_cell_t_inbounds(array_bucket_cell_t_cell_t* array, size_t i);
size_t array_bucket_cell_t_cell_t_num_els(array_bucket_cell_t_cell_t* array);
size_t array_bucket_cell_t_cell_t_capacity(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_front(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_ith(array_bucket_cell_t_cell_t* array, size_t i);
bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_back(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_end(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_front(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_ith(array_bucket_cell_t_cell_t* array, size_t i);
bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_back(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t array_bucket_cell_t_cell_t_pop(array_bucket_cell_t_cell_t* array);
void array_bucket_cell_t_cell_t_set(array_bucket_cell_t_cell_t* array, size_t i, bucket_cell_t_cell_t* el);
void array_bucket_cell_t_cell_t_grow(array_bucket_cell_t_cell_t* array);
bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_push(array_bucket_cell_t_cell_t* array, bucket_cell_t_cell_t* el);

void array_bucket_cell_t_cell_t_init(array_bucket_cell_t_cell_t* array, size_t capacity) {
    array->num_els = 0;
    array->capacity = capacity;
    array->els = (bucket_cell_t_cell_t*)malloc(capacity * sizeof(bucket_cell_t_cell_t));
}

void array_bucket_cell_t_cell_t_destroy(array_bucket_cell_t_cell_t* array) {
    if(array->els) {
        free(array->els);
    }
    array->num_els = 0;
    array->capacity = 0;
}

int array_bucket_cell_t_cell_t_empty(array_bucket_cell_t_cell_t* array) {
    return (array->num_els == 0);
}

int array_bucket_cell_t_cell_t_full(array_bucket_cell_t_cell_t* array) {
    return (array->num_els == array->capacity);
}

int array_bucket_cell_t_cell_t_inbounds(array_bucket_cell_t_cell_t* array, size_t i) {
    return (i < array->num_els);
}

size_t array_bucket_cell_t_cell_t_num_els(array_bucket_cell_t_cell_t* array) {
    return array->num_els;
}

size_t array_bucket_cell_t_cell_t_capacity(array_bucket_cell_t_cell_t* array) {
    return array->capacity;
}

bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_front(array_bucket_cell_t_cell_t* array) {
    ASSERT(!array_bucket_cell_t_cell_t_empty(array));
    return array->els;
}

bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_ith(array_bucket_cell_t_cell_t* array, size_t i) {
    ASSERT(array_bucket_cell_t_cell_t_inbounds(array, i));
    return array->els + i;
}

bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_back(array_bucket_cell_t_cell_t* array) {
    ASSERT(!array_bucket_cell_t_cell_t_empty(array));
    return array->els + array->num_els - 1;
}

bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_end(array_bucket_cell_t_cell_t* array) {
    return array_bucket_cell_t_cell_t_back(array) + 1;
}

bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_front(array_bucket_cell_t_cell_t* array) {
    return *(array_bucket_cell_t_cell_t_front(array));
}

bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_ith(array_bucket_cell_t_cell_t* array, size_t i) {
    ASSERT(array_bucket_cell_t_cell_t_inbounds(array, i));
    return *(array_bucket_cell_t_cell_t_ith(array, i));
}

bucket_cell_t_cell_t array_bucket_cell_t_cell_t_get_back(array_bucket_cell_t_cell_t* array) {
    return *(array_bucket_cell_t_cell_t_back(array));
}

bucket_cell_t_cell_t array_bucket_cell_t_cell_t_pop(array_bucket_cell_t_cell_t* array) {
    ASSERT(!array_bucket_cell_t_cell_t_empty(array));
    return array->els[(array->num_els--)];
}

void array_bucket_cell_t_cell_t_set(array_bucket_cell_t_cell_t* array, size_t i, bucket_cell_t_cell_t* el) {
    ASSERT(array_bucket_cell_t_cell_t_inbounds(array, i));
    array->els[i] = *el;
}

void array_bucket_cell_t_cell_t_grow(array_bucket_cell_t_cell_t* array) {
    bucket_cell_t_cell_t* els_new = (bucket_cell_t_cell_t*)malloc(sizeof(bucket_cell_t_cell_t) * array->num_els * 2);
    int i = 0;
    for(;i < array->num_els; ++i) {
        els_new[i] = array->els[i];
    }
    free(array->els);
    array->els = els_new;
    array->capacity = 2 * array->num_els;
}

bucket_cell_t_cell_t* array_bucket_cell_t_cell_t_push(array_bucket_cell_t_cell_t* array, bucket_cell_t_cell_t* el) {
    if(array_bucket_cell_t_cell_t_full(array)) {
        array_bucket_cell_t_cell_t_grow(array);
    }
    array->els[(array->num_els++)] = *el;
    return array_bucket_cell_t_cell_t_back(array);
}

#endif
