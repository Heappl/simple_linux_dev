#ifndef ARRAY_H
#define ARRAY_H
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif
struct array_impl;
typedef struct array_impl* array_t;

array_t array_copy(array_t other);
array_t array_create(unsigned object_size, allocator_info alloc); 
void array_destroy(array_t ctx);
void array_push(array_t ctx, void* object);
void array_pop(array_t ctx);
void* array_get(array_t ctx, unsigned index);
void array_set(array_t ctx, unsigned index, void* object);
int array_size(array_t ctx);
void array_swap_elems(array_t ctx, unsigned first, unsigned second);
#ifdef __cplusplus
}
#endif

#endif // ARRAY_H

