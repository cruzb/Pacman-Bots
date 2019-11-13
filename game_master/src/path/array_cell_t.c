#include "array_cell_t.h"
//#define cell_t uint32_t

void array_cell_t_init(array_cell_t* array, size_t capacity) {
    array->num_els = 0;
    array->capacity = capacity;
    array->els = (cell_t*)malloc(capacity * sizeof(cell_t));
}

void array_cell_t_destroy(array_cell_t* array) {
    if(array->els) {
        free(array->els);
    }
    array->num_els = 0;
    array->capacity = 0;
}

int array_cell_t_empty(array_cell_t* array) {
    return (array->num_els == 0);
}

int array_cell_t_full(array_cell_t* array) {
    return (array->num_els == array->capacity);
}

int array_cell_t_inbounds(array_cell_t* array, size_t i) {
    return (i < array->num_els);
}

size_t array_cell_t_num_els(array_cell_t* array) {
    return array->num_els;
}

size_t array_cell_t_capacity(array_cell_t* array) {
    return array->capacity;
}

cell_t* array_cell_t_front(array_cell_t* array) {
    assert(!array_cell_t_empty(array));
    return array->els;
}

cell_t* array_cell_t_ith(array_cell_t* array, size_t i) {
    assert(array_cell_t_inbounds(array, i));
    return array->els + i;
}

cell_t* array_cell_t_back(array_cell_t* array) {
    assert(!array_cell_t_empty(array));
    return array->els + array->num_els - 1;
}

cell_t* array_cell_t_end(array_cell_t* array) {
    return array_cell_t_back(array) + 1;
}

cell_t array_cell_t_get_front(array_cell_t* array) {
    return *(array_cell_t_front(array));
}

cell_t array_cell_t_get_ith(array_cell_t* array, size_t i) {
    assert(array_cell_t_inbounds(array, i));
    return *(array_cell_t_ith(array, i));
}

cell_t array_cell_t_get_back(array_cell_t* array) {
    return *(array_cell_t_back(array));
}

cell_t array_cell_t_pop(array_cell_t* array) {
    assert(!array_cell_t_empty(array));
    return array->els[(array->num_els--)];
}

void array_cell_t_set(array_cell_t* array, size_t i, cell_t* el) {
    assert(array_cell_t_inbounds(array, i));
    array->els[i] = *el;
}

void array_cell_t_grow(array_cell_t* array) {
    cell_t* els_new = (cell_t*)malloc(sizeof(cell_t) * array->num_els * 2);
    int i = 0;
    for(;i < array->num_els; ++i) {
        els_new[i] = array->els[i];
    }
    free(array->els);
    array->els = els_new;
    array->capacity = 2 * array->num_els;
}

cell_t* array_cell_t_push(array_cell_t* array, cell_t* el) {
    if(array_cell_t_full(array)) {
        array_cell_t_grow(array);
    }
    array->els[(array->num_els++)] = *el;
    return array_cell_t_back(array);
}
