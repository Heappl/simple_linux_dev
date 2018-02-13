#ifndef MEDIAN_CONTAINER_H
#define MEDIAN_CONTAINER_H

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct median_container_impl;
typedef struct median_container_impl* median_container_t;

typedef int (*elem_cmp)(void*, void*);

median_container_t median_container_create(
    unsigned elem_size, elem_cmp less, elem_cmp greater, allocator_info alloc);
void median_container_destroy(median_container_t);
void median_container_insert(median_container_t ctx, void*);
void* median_container_get_lower(median_container_t ctx);
void* median_container_get_upper(median_container_t ctx);

#ifdef __cplusplus
}
#endif
#endif // MEDIAN_CONTAINER_H

