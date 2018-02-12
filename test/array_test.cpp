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
            {
                int* elem = (int*)array_get(array, j);
                ASSERT_NE(nullptr, elem);
                ASSERT_EQ(expected[j], *elem) << j << "th element";
            }
        }
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
    auto array = array_create(sizeof(int), alloc);
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
    {
        int x = (i * 97) % 100;
        int* ptr = (int*)array_get(array, i);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ(x, *ptr) << i << "th element";
    }
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
            for (int k = 0; k < 100; ++k) {
                int* got = (int*)array_get(array, k);
                ASSERT_NE(nullptr, got);
                ASSERT_EQ(expected[k], *got) << j << "th index";
            }
        }
    }
}

} //namespace

