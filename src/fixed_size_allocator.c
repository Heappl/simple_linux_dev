#include "fixed_size_allocator.h"

struct fs_allocator_info_impl
{
    allocator_info sys_alloc;
    unsigned chunk_size;
    unsigned resize_by;
    void* allocated;
    void* next;
};

void* fs_alloc_impl_prepare_chunk(void* chunk, unsigned size, unsigned chunk_size, void** prev)
{
    int i;
    for (i = 0; i < size; ++i) {
        *(void**)chunk = *prev;
        *prev = chunk;
        chunk += chunk_size;
    }
    return chunk;
}

fs_allocator_info fs_alloc_create(int chunk_size, allocator_info sys_alloc, int resize_by)
{
    fs_allocator_info ret = allocate(sys_alloc, sizeof(struct fs_allocator_info_impl));
    ret->sys_alloc = sys_alloc;
    ret->chunk_size = chunk_size;
    ret->resize_by = resize_by;
    ret->next = 0;
    ret->allocated = 0;
    return ret;
}

int fs_alloc_destroy(fs_allocator_info alloc)
{
    void* allocated = alloc->allocated;
    while (allocated)
    {
        void* next = *(void**)allocated;
        deallocate(alloc->sys_alloc, allocated);
        allocated = next;
    }
    deallocate(alloc->sys_alloc, alloc);
}

void fs_alloc_impl_increase(fs_allocator_info alloc)
{
    void* next_chunk =
        allocate(alloc->sys_alloc, alloc->chunk_size * (alloc->resize_by + 1));
    *(void**)next_chunk = alloc->allocated;
    alloc->allocated = next_chunk;
    fs_alloc_impl_prepare_chunk(next_chunk + alloc->chunk_size, alloc->resize_by, alloc->chunk_size, &alloc->next);
}

void* fs_alloc_allocate(fs_allocator_info alloc)
{
    if (!alloc->next)
        fs_alloc_impl_increase(alloc);
    void* ret = alloc->next;
    alloc->next = *(void**)alloc->next;
    return ret;
}

void fs_alloc_deallocate(fs_allocator_info alloc, void* ptr)
{
    *(void**)ptr = alloc->next;
    alloc->next = ptr;
}

