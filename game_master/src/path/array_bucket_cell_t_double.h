#ifndef ARRAY_BUCKET_CELL_T_DOUBLE_H
#define ARRAY_BUCKET_CELL_T_DOUBLE_H

#include <inttypes.h>
#include <stdlib.h>
#include "CMSIS/mss_assert.h"
//#define bucket_cell_t_double uint32_t

typedef struct array_bucket_cell_t_double {
    size_t num_els;
    size_t capacity;
    bucket_cell_t_double* els;
} array_bucket_cell_t_double;

void array_bucket_cell_t_double_init(array_bucket_cell_t_double* array, size_t capacity);
void array_bucket_cell_t_double_destroy(array_bucket_cell_t_double* array);
int array_bucket_cell_t_double_empty(array_bucket_cell_t_double* array);
int array_bucket_cell_t_double_full(array_bucket_cell_t_double* array);
int array_bucket_cell_t_double_inbounds(array_bucket_cell_t_double* array, size_t i);
size_t array_bucket_cell_t_double_num_els(array_bucket_cell_t_double* array);
size_t array_bucket_cell_t_double_capacity(array_bucket_cell_t_double* array);
bucket_cell_t_double* array_bucket_cell_t_double_front(array_bucket_cell_t_double* array);
bucket_cell_t_double* array_bucket_cell_t_double_ith(array_bucket_cell_t_double* array, size_t i);
bucket_cell_t_double* array_bucket_cell_t_double_back(array_bucket_cell_t_double* array);
bucket_cell_t_double* array_bucket_cell_t_double_end(array_bucket_cell_t_double* array);
bucket_cell_t_double array_bucket_cell_t_double_get_front(array_bucket_cell_t_double* array);
bucket_cell_t_double array_bucket_cell_t_double_get_ith(array_bucket_cell_t_double* array, size_t i);
bucket_cell_t_double array_bucket_cell_t_double_get_back(array_bucket_cell_t_double* array);
bucket_cell_t_double array_bucket_cell_t_double_pop(array_bucket_cell_t_double* array);
void array_bucket_cell_t_double_set(array_bucket_cell_t_double* array, size_t i, bucket_cell_t_double* el);
void array_bucket_cell_t_double_grow(array_bucket_cell_t_double* array);
bucket_cell_t_double* array_bucket_cell_t_double_push(array_bucket_cell_t_double* array, bucket_cell_t_double* el);

void array_bucket_cell_t_double_init(array_bucket_cell_t_double* array, size_t capacity) {
    array->num_els = 0;
    array->capacity = capacity;
    array->els = (bucket_cell_t_double*)malloc(capacity * sizeof(bucket_cell_t_double));
}

void array_bucket_cell_t_double_destroy(array_bucket_cell_t_double* array) {
    if(array->els) {
        free(array->els);
    }
    array->num_els = 0;
    array->capacity = 0;
}

int array_bucket_cell_t_double_empty(array_bucket_cell_t_double* array) {
    return (array->num_els == 0);
}

int array_bucket_cell_t_double_full(array_bucket_cell_t_double* array) {
    return (array->num_els == array->capacity);
}

int array_bucket_cell_t_double_inbounds(array_bucket_cell_t_double* array, size_t i) {
    return (i < array->num_els);
}

size_t array_bucket_cell_t_double_num_els(array_bucket_cell_t_double* array) {
    return array->num_els;
}

size_t array_bucket_cell_t_double_capacity(array_bucket_cell_t_double* array) {
    return array->capacity;
}

bucket_cell_t_double* array_bucket_cell_t_double_front(array_bucket_cell_t_double* array) {
    ASSERT(!array_bucket_cell_t_double_empty(array));
    return array->els;
}

bucket_cell_t_double* array_bucket_cell_t_double_ith(array_bucket_cell_t_double* array, size_t i) {
    ASSERT(array_bucket_cell_t_double_inbounds(array, i));
    return array->els + i;
}

bucket_cell_t_double* array_bucket_cell_t_double_back(array_bucket_cell_t_double* array) {
    ASSERT(!array_bucket_cell_t_double_empty(array));
    return array->els + array->num_els - 1;
}

bucket_cell_t_double* array_bucket_cell_t_double_end(array_bucket_cell_t_double* array) {
    return array_bucket_cell_t_double_back(array) + 1;
}

bucket_cell_t_double array_bucket_cell_t_double_get_front(array_bucket_cell_t_double* array) {
    return *(array_bucket_cell_t_double_front(array));
}

bucket_cell_t_double array_bucket_cell_t_double_get_ith(array_bucket_cell_t_double* array, size_t i) {
    ASSERT(array_bucket_cell_t_double_inbounds(array, i));
    return *(array_bucket_cell_t_double_ith(array, i));
}

bucket_cell_t_double array_bucket_cell_t_double_get_back(array_bucket_cell_t_double* array) {
    return *(array_bucket_cell_t_double_back(array));
}

bucket_cell_t_double array_bucket_cell_t_double_pop(array_bucket_cell_t_double* array) {
    ASSERT(!array_bucket_cell_t_double_empty(array));
    return array->els[(array->num_els--)];
}

void array_bucket_cell_t_double_set(array_bucket_cell_t_double* array, size_t i, bucket_cell_t_double* el) {
    ASSERT(array_bucket_cell_t_double_inbounds(array, i));
    array->els[i] = *el;
}

void array_bucket_cell_t_double_grow(array_bucket_cell_t_double* array) {
    bucket_cell_t_double* els_new = (bucket_cell_t_double*)malloc(sizeof(bucket_cell_t_double) * array->num_els * 2);
    int i = 0;
    for(;i < array->num_els; ++i) {
        els_new[i] = array->els[i];
    }
    free(array->els);
    array->els = els_new;
    array->capacity = 2 * array->num_els;
}

bucket_cell_t_double* array_bucket_cell_t_double_push(array_bucket_cell_t_double* array, bucket_cell_t_double* el) {
    if(array_bucket_cell_t_double_full(array)) {
        array_bucket_cell_t_double_grow(array);
    }
    array->els[(array->num_els++)] = *el;
    return array_bucket_cell_t_double_back(array);
}

#endif
