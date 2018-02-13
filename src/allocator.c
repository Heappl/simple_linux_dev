#include "allocator.h"
#include "linux_wrapper.h"

void* allocate(allocator_info ctx, unsigned size)
{
    return ctx.allocate(ctx.ctx, size);
}

void deallocate(allocator_info ctx, void* ptr)
{
    return ctx.deallocate(ctx.ctx, ptr);
}

void* kernel_allocate_wrapper(void* ctx, unsigned size)
{
    (void)ctx;
    return kernel_alloc(size);
}
void kernel_deallocate_wrapper(void* ctx, void* ptr)
{
    (void)ctx;
    return kernel_dealloc(ptr);
}

allocator_info make_kernel_alloc(void)
{
    allocator_info ret = {
        .ctx = 0,
        .allocate = &kernel_allocate_wrapper,
        .deallocate = &kernel_deallocate_wrapper
    };
    return ret;
}

