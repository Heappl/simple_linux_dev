#include "allocator.h"

struct AllocatorCtx
{
    allocator_info sys_alloc;
};

static void* allocate(void* pctx)
{
    AllocatorCtx* ctx = (AllocatorCtx*)pctx;
    return ctx->sys_alloc.allocate(ctx->sys_alloc.ctx);
}

static void* deallocate(void* ctx, void* ptr)
{
    return ctx->sys_alloc.deallocate(ctx->sys_alloc.ptr);
}

allocator_info allocator_create(int chunk_size, allocator_info sys_alloc)
{
    assert(sys_alloc.chunk_size >= sizeof(AllocatorCtx));
    AllocatorCtx* ctx = (void*)sys_alloc.allocate(sys_alloc.ctx);
    ctx->sys_alloc = sys_alloc;
    allocator_info ret = {
        .ctx = ctx,
        .allocate = &allocate,
        .deallocate = &deallocate,
        .chunk_size = chunk_size
    };
    return ret;
}

allocator allocator_destroy(allocator_info alloc)
{
    
}

