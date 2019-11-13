#include "map_cell_t_double.h"

void map_cell_t_double_destroy(map_cell_t_double* map) {
    array_bucket_cell_t_double_destroy(&(map->buckets));
}

void map_cell_t_double_init(map_cell_t_double* map, 
                size_t num_buckets,
                size_t(*hash)(cell_t),
                int(*match)(cell_t, cell_t),
                cell_t(*default_key)(void),
                double(*default_val)(void))
{
    map->hash = hash;
    map->match = match;
    map->default_key = default_key;
    map->default_val = default_val;
    array_bucket_cell_t_double* buckets = &(map->buckets);
    array_bucket_cell_t_double_init(buckets, num_buckets);

    bucket_cell_t_double default_bucket = (bucket_cell_t_double){1, map->default_key(), map->default_val()};
    int i = 0;
    for(; i < num_buckets; ++i) {
        array_bucket_cell_t_double_push(buckets, &default_bucket);
    }
    assert(array_bucket_cell_t_double_full(buckets));
}

size_t map_cell_t_double_num_buckets(map_cell_t_double* map) {
    //return array_bucket_cell_t_double_capacity(&(map->buckets));
    return array_bucket_cell_t_double_num_els(&(map->buckets));
}

size_t map_cell_t_double_hash(map_cell_t_double* map, cell_t key) {
    return (map->hash(key) % map_cell_t_double_num_buckets(map));
}

int map_cell_t_double_match(map_cell_t_double* map, cell_t a, cell_t b) {
    return map->match(a, b);
}

bucket_cell_t_double* map_cell_t_double_find(map_cell_t_double* map, cell_t key) {
    array_bucket_cell_t_double* buckets = &(map->buckets);
    bucket_cell_t_double* bucket = array_bucket_cell_t_double_ith(buckets, map_cell_t_double_hash(map, key));
    bucket_cell_t_double* end = array_bucket_cell_t_double_end(buckets);
    for(; bucket != end; ++bucket) {
        if(!bucket->empty && map_cell_t_double_match(map, bucket->key, key)) {
            return bucket;
        }
    }
    return NULL;
}

void map_cell_t_double_grow(map_cell_t_double* map) {
    map_cell_t_double new_map;
    map_cell_t_double_init(&new_map, 
                2*map_cell_t_double_num_buckets(map),
                map->hash,
                map->match,
                map->default_key,
                map->default_val);
    bucket_cell_t_double* bucket = array_bucket_cell_t_double_front(&(map->buckets));
    bucket_cell_t_double* end = array_bucket_cell_t_double_end(&(map->buckets));
    for(; bucket != end; ++bucket) {
        *map_cell_t_double_put(&new_map, bucket->key) = bucket->val;
    }
    map_cell_t_double_destroy(map);
    *map = new_map;
}

double* map_cell_t_double_put(map_cell_t_double* map, cell_t key) {
    bucket_cell_t_double new_bucket = (bucket_cell_t_double){0, key, map->default_val()};
    array_bucket_cell_t_double* buckets = &(map->buckets);
    bucket_cell_t_double* bucket = array_bucket_cell_t_double_ith(buckets, map_cell_t_double_hash(map, key));
    bucket_cell_t_double* end = array_bucket_cell_t_double_end(buckets);
    for(; bucket != end; ++bucket) {
        if(bucket->empty) {
            *bucket = new_bucket;
            return &(bucket->val);
        }
    }
    map_cell_t_double_grow(map);
    return map_cell_t_double_put(map, key);
}

double* map_cell_t_double_get(map_cell_t_double* map, cell_t key) {
    bucket_cell_t_double* bucket = map_cell_t_double_find(map, key);
    if(bucket) {
        return &(bucket->val);
    }
    else {
        return map_cell_t_double_put(map, key);
    }
}

void map_cell_t_double_remove(map_cell_t_double* map, cell_t key) {
    bucket_cell_t_double* bucket = map_cell_t_double_find(map, key);
    if(bucket) {
        bucket->empty = 1;
    }
}
