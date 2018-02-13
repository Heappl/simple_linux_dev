#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "fixed_size_allocator.h"
#include <unordered_set>
#include <algorithm>

namespace
{
using namespace ::testing;

struct AllocatorTest : Test
{
    allocator_info stub_alloc = {
        (void*)this,
        [](void* ctx, unsigned size) -> void* {
            return ((AllocatorTest*)ctx)->next_buffer(size);
        },
        [](void* ctx, void* ptr){
            ((AllocatorTest*)ctx)->return_buffer(ptr);
        }
    };

    int chunk_size = sizeof(long);
    int resize_by = 5;
    int required = ((sizeof(allocator_info) + sizeof(void*)) + chunk_size - 1) / chunk_size;
    std::unordered_set<void*> allocated;
    size_t max_sofar = 0;
    void* next_buffer(unsigned size)
    {
        //assert(size == (resize_by + 1) * chunk_size);
        void* ret = malloc(size);
        allocated.insert(ret);
        return ret;
    }
    void return_buffer(void* ptr)
    {
        auto it = allocated.find(ptr);
        assert(it != allocated.end());
        allocated.erase(it);
        free(ptr);
    }
    fs_allocator_info alloc;
    
    AllocatorTest()
    {
        alloc = fs_alloc_create(chunk_size, stub_alloc, resize_by);
    }

    void release_and_check()
    {
        fs_alloc_destroy(alloc);
        ASSERT_TRUE(allocated.empty());
    }
    void check_allocated_from_sys()
    {
        ASSERT_LT(required + sofar.size(), allocated.size() * resize_by);
        ASSERT_EQ((max_sofar + resize_by - 1) / resize_by + 1,
                  allocated.size());
    }

    std::unordered_set<void*> sofar;
    void allocateSome(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            void* allocated = fs_alloc_allocate(alloc);
            ASSERT_EQ(sofar.end(), sofar.find(allocated));
            sofar.insert(allocated);
            max_sofar = std::max(sofar.size(), max_sofar);
            memset(allocated, 0, chunk_size);
            check_allocated_from_sys();
        }
    }
    void freeSome(int n)
    {
        for (int i = 0; (i < n) and not sofar.empty(); ++i)
        {
            fs_alloc_deallocate(alloc, *sofar.begin());
            sofar.erase(sofar.begin());
            check_allocated_from_sys();
        }
    }
    void allocateSomeAndFreeAll()
    {
        allocateSome(100);
        freeSome(sofar.size());
    }
};

TEST_F(AllocatorTest, SingleAllocAndFree)
{
    void* allocated = fs_alloc_allocate(alloc);
    ASSERT_NE(nullptr, allocated);
    fs_alloc_deallocate(alloc, allocated);
    ASSERT_EQ(allocated, fs_alloc_allocate(alloc));
    fs_alloc_deallocate(alloc, allocated);
    release_and_check();
}

TEST_F(AllocatorTest, allocateMultipleAndFreeAll)
{
    for (int i = 0; i < 10; ++i)
        allocateSomeAndFreeAll();
    release_and_check();
}

TEST_F(AllocatorTest, allocateSomeAndDeallocateSomeRandomAndRepeat)
{
    for (int i = 0; i < 10; ++i)
    {
        allocateSome(1000);
        freeSome((213 * i) % sofar.size());
    }
    freeSome(sofar.size());
    release_and_check();
}

} //namespace

