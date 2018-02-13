#ifndef FIXED_SIZE_ALLOCATOR_H
#define FIXED_SIZE_ALLOCATOR_H
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif
struct fs_allocator_info_impl;
typedef struct fs_allocator_info_impl* fs_allocator_info;

/*
 * minimal chunks size is sizeof(void*)
 * and resize_by * chunk_size must cover the internal info,
 * which sizeof(allocator_info) + sizeof(void*) * 2
 * each system allocated chunk will be increased by one chunk size for internal purpose
 */
fs_allocator_info fs_alloc_create(int chunk_size, allocator_info sys_alloc, int resize_by);
int fs_alloc_destroy(fs_allocator_info alloc);

void* fs_alloc_allocate(fs_allocator_info alloc);
void fs_alloc_deallocate(fs_allocator_info alloc, void*);

#ifdef __cplusplus
}
#endif

#endif //FIXED_SIZE_ALLOCATOR_H

