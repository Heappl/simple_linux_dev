#ifndef ALLOCATOR_H
#define ALLOCATOR_H

typedef void* (*allocate_func)(void* ctx, unsigned size);
typedef void  (*deallocate_func)(void* ctx, void*);
typedef struct
{
    void* ctx;
    allocate_func allocate;
    deallocate_func deallocate;
} allocator_info;

#endif //ALLOCATOR_H

