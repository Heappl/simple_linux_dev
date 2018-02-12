#include "gtest/gtest.h"
#include "allocator.h"
#include "array.h"
#include "heap.h"
#include <algorithm>

namespace
{

struct HeapTest : ::testing::Test
{
    allocator_info alloc = {
        nullptr,
        [](void*, unsigned size){ return malloc(size); },
        [](void*, void* ptr){ free(ptr); }
    };

    heap_cmp less = [](void* left, void* right) -> int {
        return *(int*)left < *(int*)right;
    };
    heap_cmp greater = [](void* left, void* right) -> int {
        return *(int*)left > *(int*)right;
    };

    array_t array = array_create(sizeof(int), alloc);
    ~HeapTest()
    {
        array_destroy(array);
    }
};

TEST_F(HeapTest, singlePush) {
    int x = 11;
    heap_push(array, &x, less);
    ASSERT_EQ(1, array_size(array));
    ASSERT_EQ(11, *(int*)array_get(array, 0));
}

TEST_F(HeapTest, manyUnorderedPushes)
{
    std::vector<int> vals = {4, 99, 97, 96, 101, 0, 76, 999, 998};
    std::vector<int> expected[] = {
        {4},
        {99, 4},
        {99, 4, 97},
        {99, 96, 97, 4},
        {101, 99, 97, 4, 96},
        {101, 99, 97, 4, 96, 0},
        {101, 99, 97, 4, 96, 0, 76},
        {999, 101, 97, 99, 96, 0, 76, 4},
        {999, 998, 97, 101, 96, 0, 76, 4, 99},
    };
    for (int i = 0; i < vals.size(); ++i)
    {
        heap_push(array, &vals[i], less);
        ASSERT_EQ(expected[i].size(), array_size(array));
        for (int j = 0; j < expected[i].size(); ++j)
        {
            int* x = (int*)array_get(array, j);
            ASSERT_NE(nullptr, x);
            ASSERT_EQ(expected[i][j], *x) << i << " iteration, " << j << " index";
        }
    }
}
} //namespace

