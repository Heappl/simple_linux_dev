#include "array.h"
#include "linux_wrapper.h"

struct array_impl
{
    allocator_info alloc;
    int obj_size;
    void* object;
    int available;
    int size;
};

array_t array_copy(array_t other)
{
    array_t ret = allocate(other->alloc, sizeof(struct array_impl));
    *ret = *other;
    ret->object = allocate(other->alloc, other->available * other->obj_size);
    copy(ret->object, other->object, other->size * other->obj_size);
    return ret;
}

array_t array_create(unsigned object_size, allocator_info alloc)
{
    array_t ret = allocate(alloc, sizeof(struct array_impl));
    ret->obj_size = object_size;
    ret->size = 0;
    ret->alloc = alloc;
    ret->available = 1;
    ret->object = allocate(alloc, object_size);
    return ret;
}

void array_destroy(array_t ctx)
{
    deallocate(ctx->alloc, ctx->object);
    deallocate(ctx->alloc, ctx);
}

void array_push(array_t ctx, void* object)
{
    if (ctx->available == ctx->size)
    {
        int next_available = ctx->available * 2 + 1;
        int to_alloc_size = next_available * ctx->obj_size;
        void* next_buffer = ctx->alloc.allocate(ctx->alloc.ctx, to_alloc_size);
        if (!next_buffer || next_available < ctx->available)
        {
            kernel_error("memory allocation failed");
            return;
        }
        copy(next_buffer, ctx->object, ctx->available * ctx->obj_size);
        ctx->alloc.deallocate(ctx->alloc.ctx, ctx->object);
        ctx->available = next_available;
        ctx->object = next_buffer;
    }
    copy(ctx->object + ctx->size * ctx->obj_size, object, ctx->obj_size);
    ctx->size++;
}

void array_pop(array_t ctx)
{
    if (ctx->size > 0) ctx->size--;
}

void* array_get(array_t ctx, unsigned index)
{
    if (index >= ctx->size) return 0;
    return ctx->object + index * ctx->obj_size;
}

void array_set(array_t ctx, unsigned index, void* object)
{
    if (index < ctx->size)
        copy(ctx->object + index * ctx->obj_size, object, ctx->obj_size);
}

int array_size(array_t ctx)
{
    return ctx->size;
}

void array_swap_elems(array_t ctx, unsigned first, unsigned second)
{
    char* first_elem;
    char* second_elem;
    int i;
    if ((first >= ctx->size) || (second >= ctx->size))
        return;
    first_elem = ctx->object + first * ctx->obj_size;
    second_elem = ctx->object + second * ctx->obj_size;
    for (i = 0; i < ctx->obj_size; ++i)
    {
        *(first_elem + i) ^= *(second_elem + i);
        *(second_elem + i) ^= *(first_elem + i);
        *(first_elem + i) ^= *(second_elem + i);
    }
}

