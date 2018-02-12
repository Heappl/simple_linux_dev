#include "heap.h"

void heap_push(array_t array, void* elem, heap_cmp less)
{
    array_push(array, elem);
    int index = array_size(array) - 1;
    while (index > 0) {
        int parent_index = (index + 1) / 2 - 1;
        void* parent = array_get(array, parent_index);
        if (less(elem, parent)) 
        {
            array_set(array, index, elem);
            break;
        }
        array_set(array, index, parent);
        index = parent_index;
    }
    array_set(array, index, elem);
}

void heap_pop(array_t array, void* elem, heap_cmp less)
{
}

