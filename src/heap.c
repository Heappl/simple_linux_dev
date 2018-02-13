#include "heap.h"

void heap_push(array_t array, heap_cmp less)
{
    int index = array_size(array) - 1;
    while (index > 0) {
        int parent_index = (index + 1) / 2 - 1;
        void* parent = array_get(array, parent_index);
        void* elem = array_get(array, index);
        if (less(elem, parent)) 
            break;
        array_swap_elems(array, index, parent_index);
        index = parent_index;
    }
}

int heap_aux_choose_bigger_child(array_t array, int upto, int index, heap_cmp less)
{
    int first_child_index = (index + 1) * 2 - 1;
    int second_child_index = (index + 1) * 2;
    if (first_child_index >= upto)
        return -1;
    if (second_child_index >= upto)
        return first_child_index;
    void* first_child = array_get(array, first_child_index);
    void* second_child = array_get(array, second_child_index);
    if (less(first_child, second_child)) return second_child_index;
    return first_child_index;
}

void heap_pop(array_t array, heap_cmp less)
{
    int size = array_size(array) - 1;
    if (size <= 0) return;
    array_swap_elems(array, 0, size);
    int index = 0;
    while (index < size) {
        int next_index = heap_aux_choose_bigger_child(array, size, index, less);
        if (next_index < 0)
            break;
        void* curr = array_get(array, index);
        void* next = array_get(array, next_index);
        if (less(next, curr))
            break;
        array_swap_elems(array, index, next_index);
        index = next_index;
    }
}

