#include "gtest/gtest.h"
#include "allocator.h"
#include "array.h"
#include <algorithm>

namespace
{

struct ArrayTest : ::testing::Test
{
    allocator_info alloc = {
        nullptr,
        [](void*, unsigned size){ return malloc(size); },
        [](void*, void* ptr){ free(ptr); }
    };

    array_t array = array_create(sizeof(int), alloc);
    ~ArrayTest()
    {
        array_destroy(array);
    }
    
    void check_with_vector(int push_balance)
    {
        std::vector<int> expected;
        for (int i = 0; i < 1000; ++i)
        {
            int elem = (i * 97 + 101) % 10231;
            int action = (elem * 12321 / 5) % 4;
            if (action > push_balance)
            {
                expected.push_back(elem);
                array_push(array, &elem);
            }
            else if (not expected.empty())
            {
                expected.pop_back();
                array_pop(array);
            }
            ASSERT_EQ(expected.size(), array_size(array));
            for (int j = 0; j < expected.size(); ++j)
                check_elem(j, expected[j]);
        }
    }

    void check_elem(int index, int expected)
    {
        int* got = (int*)array_get(array, index);
        ASSERT_NE(nullptr, got);
        ASSERT_EQ(expected, *got) << index << "th index";
    }
};

TEST_F(ArrayTest, emptyPop) {
    ASSERT_EQ(0, array_size(array));
    array_pop(array);
    ASSERT_EQ(0, array_size(array));
}

TEST_F(ArrayTest, singlePushAndPop) {
    int x = 11;
    array_push(array, &x);
    ASSERT_EQ(1, array_size(array));
    ASSERT_EQ(11, *(int*)array_get(array, 0));
    array_pop(array);
    ASSERT_EQ(0, array_size(array));
}

TEST_F(ArrayTest, manyPushesFollowedByPopsTillEmpty_SizeCheck) {
    for (int i = 0; i < 100; ++i) {
        array_push(array, &i);
        ASSERT_EQ(i + 1, array_size(array));
    }
    for (int i = 100; i > -10; --i) {
        array_pop(array);
        ASSERT_EQ(std::max(i - 1, 0), array_size(array));
    }
}

TEST_F(ArrayTest, manyPushes_checkingAllElementsAfter) {
    for (int i = 0; i < 100; ++i)
    {
        int x = (i * 97) % 100;
        array_push(array, &x);
    }
    for (int i = 0; i < 100; ++i)
        check_elem(i, (i * 97) % 100);
}

TEST_F(ArrayTest, moreRandomPushesAndPops_checkingElementsWithVector)
{
    check_with_vector(0);
}
TEST_F(ArrayTest, randomPushesAndMorePops_checkingElementsWithVector)
{
    check_with_vector(2);
}
TEST_F(ArrayTest, randomPushesAndPops_checkingElementsWithVector)
{
    check_with_vector(1);
}

TEST_F(ArrayTest, setOnEmpty)
{
    int x = 1;
    array_set(array, 0, &x);
    ASSERT_EQ(0, array_size(array));
    ASSERT_EQ(nullptr, array_get(array, 0));
}
TEST_F(ArrayTest, randomSets)
{
    std::vector<int> expected;
    for (int i = 0; i < 100; ++i)
    {
        expected.push_back(i);
        array_push(array, &i);
    }
    ASSERT_EQ(expected.size(), array_size(array));
    for (int j = 0; j <= 1000; ++j)
    {
        int index = ((j + 12311) * 97 + 77) % 100;
        int elem = expected[(index + 99) % 100] * j + index; 
        expected[index] = elem;
        array_set(array, index, &elem);
        
        if (j % 10 == 0) {
            for (int k = 0; k < 100; ++k)
                check_elem(k, expected[k]);
        }
    }
}

TEST_F(ArrayTest, swapElements)
{
    for (int i = 0; i < 100; ++i)
        array_push(array, &i);
    array_swap_elems(array, 5, 18);
    ASSERT_NO_FATAL_FAILURE(check_elem(5, 18));
    ASSERT_NO_FATAL_FAILURE(check_elem(18, 5));
}
TEST_F(ArrayTest, swapInvalidElements)
{
    for (int i = 0; i < 10; ++i)
        array_push(array, &i);
    array_swap_elems(array, 11, 8);
    array_swap_elems(array, 1, 15);
    ASSERT_NO_FATAL_FAILURE(check_elem(1, 1));
    ASSERT_NO_FATAL_FAILURE(check_elem(8, 8));
}
} //namespace

