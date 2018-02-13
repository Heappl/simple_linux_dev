#ifndef HEAP_H
#define HEAP_H
#include "array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*heap_cmp)(void*, void*);

/*
* Perform heap push operator on a array.
* The element will be added to array at the end,
* and then moved up to the proper position,
* based on comparison function,
* when the heap_cmp(elem, parent) is true, it will stop
*/
void heap_push(array_t, heap_cmp);

/*
* Perform heap pop operator on a array.
* The maximum element (in regard to heap_cmp),
* will moved down to the last position
* and the remaining heap will be fixed.
* The element will stay in the last position.
*/
void heap_pop(array_t, heap_cmp);

#ifdef __cplusplus
}
#endif

#endif //HEAP_H

