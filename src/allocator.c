#include "allocator.h"

void* allocate(allocator_info ctx, unsigned size)
{
    return ctx.allocate(ctx.ctx, size);
}

void deallocate(allocator_info ctx, void* ptr)
{
    return ctx.deallocate(ctx.ctx, ptr);
}

