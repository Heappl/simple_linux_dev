#ifndef HEAP_H
#define HEAP_H
#include "array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*heap_cmp)(void*, void*);

void heap_push(array_t, void*, heap_cmp);
void heap_pop(array_t, void*, heap_cmp);

#ifdef __cplusplus
}
#endif

#endif //HEAP_H

