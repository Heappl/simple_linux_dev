#include "median_container.h"
#include "array.h"
#include "heap.h"

struct median_container_impl
{
    array_t smaller, bigger;
    elem_cmp less, greater;
    allocator_info alloc;
};

median_container_t median_container_create(
    unsigned elem_size, elem_cmp less, elem_cmp greater, allocator_info alloc)
{
    median_container_t ret = allocate(alloc, sizeof(struct median_container_impl));
    ret->smaller = array_create(elem_size, alloc);
    ret->bigger = array_create(elem_size, alloc);
    ret->less = less;
    ret->greater = greater;
    ret->alloc = alloc;
    return ret;
}

void median_container_destroy(median_container_t ctx)
{
    array_destroy(ctx->smaller);
    array_destroy(ctx->bigger);
    deallocate(ctx->alloc, ctx);
}

void median_container_insert(median_container_t ctx, void* elem)
{
    void* max_smaller = array_get(ctx->smaller, 0);
    if (max_smaller && ctx->less(max_smaller, elem))
    {
        array_push(ctx->bigger, elem);
        heap_push(ctx->bigger, ctx->greater);
    }
    else
    {
        array_push(ctx->smaller, elem);
        heap_push(ctx->smaller, ctx->less);
    }

    while (array_size(ctx->smaller) > array_size(ctx->bigger) + 1)
    {
        void* max_smaller = array_get(ctx->smaller, 0);
        array_push(ctx->bigger, max_smaller);
        heap_push(ctx->bigger, ctx->greater);
        heap_pop(ctx->smaller, ctx->less);
        array_pop(ctx->smaller);
    }
    while (array_size(ctx->bigger) > array_size(ctx->smaller))
    {
        void* min_bigger = array_get(ctx->bigger, 0);
        array_push(ctx->smaller, min_bigger);
        heap_push(ctx->smaller, ctx->less);
        heap_pop(ctx->bigger, ctx->greater);
        array_pop(ctx->bigger);
    }
}

void* median_container_get_lower(median_container_t ctx)
{
    if (array_size(ctx->smaller) == 0)
        return 0;
    return array_get(ctx->smaller, 0);
}

void* median_container_get_upper(median_container_t ctx)
{
    if (array_size(ctx->smaller) == 0)
        return 0;
    if (array_size(ctx->smaller) == array_size(ctx->bigger))
        return array_get(ctx->bigger, 0);
    return array_get(ctx->smaller, 0);
}


