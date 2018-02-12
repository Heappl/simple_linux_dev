#ifndef FIXED_SIZE_ALLOCATOR_H
#define FIXED_SIZE_ALLOCATOR_H

typedef void* (allocate_func)(void* ctx);
typedef void  (deallocate_func)(void* ctx, void*);
typedef struct
{
    void* ctx;
    allocate_func allocate;
    deallocate_func deallocate;
    unsigned chunk_size;
} allocator_info;

allocator_info allocator_create(int chunk_size, allocator_info sys_alloc);
allocator_info allocator_destroy(allocator_info alloc);

#endif //FIXED_SIZE_ALLOCATOR_H

