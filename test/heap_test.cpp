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

    void check_array_content(std::vector<int> expected)
    {
        ASSERT_EQ(expected.size(), array_size(array));
        for (int j = 0; j < expected.size(); ++j)
        {
            int* x = (int*)array_get(array, j);
            ASSERT_NE(nullptr, x);
            ASSERT_EQ(expected[j], *x) << j << " index";
        }
    }

    void fill_array(std::vector<int> expected)
    {
        for (int elem : expected)
            array_push(array, &elem);
    }
};

TEST_F(HeapTest, singlePush) {
    int x = 11;
    array_push(array, &x);
    heap_push(array, less);
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
        array_push(array, &vals[i]);
        heap_push(array, less);
        ASSERT_NO_FATAL_FAILURE(check_array_content(expected[i])) << i << " iteration, ";
    }
}

TEST_F(HeapTest, popOnEmpty)
{
    heap_pop(array, greater);
    ASSERT_NO_FATAL_FAILURE(check_array_content({}));
}

TEST_F(HeapTest, popOnSingleElementHeap)
{
    fill_array({1});
    heap_pop(array, greater);
    ASSERT_NO_FATAL_FAILURE(check_array_content({1}));
}

TEST_F(HeapTest, popOnTwoElems)
{
    fill_array({1, 2});
    heap_pop(array, greater);
    ASSERT_NO_FATAL_FAILURE(check_array_content({2, 1}));
}

TEST_F(HeapTest, popOnOnLevelTreeSimpleCase)
{
    fill_array({3, 1, 2});
    heap_pop(array, less);
    ASSERT_NO_FATAL_FAILURE(check_array_content({2, 1, 3}));
}

TEST_F(HeapTest, popOnOnLevelTreeReorderCase)
{
    fill_array({3, 2, 1});
    heap_pop(array, less);
    ASSERT_NO_FATAL_FAILURE(check_array_content({2, 1, 3}));
}

TEST_F(HeapTest, popToSingleLeafOnLevelThree)
{
    fill_array({3, 2, 1, 0});
    heap_pop(array, less);
    ASSERT_NO_FATAL_FAILURE(check_array_content({2, 0, 1, 3}));
}

TEST_F(HeapTest, popOnBiggerExample)
{
    fill_array({100, 50, 10, 47, 48, 8, 9, 44, 43, 42, 41, 7});
    heap_pop(array, less);
    ASSERT_NO_FATAL_FAILURE(check_array_content({50, 48, 10, 47, 42, 8, 9, 44, 43, 7, 41, 100}));
}

} //namespace

