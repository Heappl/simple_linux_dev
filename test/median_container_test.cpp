#include "gtest/gtest.h"
#include "allocator.h"
#include "median_container.h"
#include <algorithm>

namespace
{

struct MedianContainerTest : ::testing::Test
{
    allocator_info alloc = {
        nullptr,
        [](void*, unsigned size){ return malloc(size); },
        [](void*, void* ptr){ free(ptr); }
    };

    elem_cmp less = [](void* left, void* right) -> int {
        return *(int*)left < *(int*)right;
    };
    elem_cmp greater = [](void* left, void* right) -> int {
        return *(int*)left > *(int*)right;
    };

    median_container_t mc = median_container_create(sizeof(int), less, greater, alloc);
    ~MedianContainerTest()
    {
        median_container_destroy(mc);
    }

    void insert(int x)
    {
        median_container_insert(mc, &x);
    }

    void check_median(int lower, int upper)
    {
        int* ptr = (int*)median_container_get_lower(mc);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ(lower, *ptr);
        ptr = (int*)median_container_get_upper(mc);
        ASSERT_NE(nullptr, ptr);
        ASSERT_EQ(upper, *ptr);
    }
};

TEST_F(MedianContainerTest, checkMedianOnEmpty)
{
    ASSERT_EQ(nullptr, median_container_get_lower(mc));
    ASSERT_EQ(nullptr, median_container_get_upper(mc));
}
TEST_F(MedianContainerTest, singleInsert)
{
    insert(11);
    check_median(11, 11);
}
TEST_F(MedianContainerTest, multipleSameInsert)
{
    for (int i = 0; i < 20; ++i)
        insert(3);
    check_median(3, 3);
}
TEST_F(MedianContainerTest, twoOrderedElements)
{
    insert(1);
    insert(2);
    check_median(1, 2);
}
TEST_F(MedianContainerTest, twoUnorderedElements)
{
    insert(3);
    insert(2);
    check_median(2, 3);
}
TEST_F(MedianContainerTest, threeOrderedElemets)
{
    insert(1);
    insert(2);
    insert(3);
    check_median(2, 2);
}
TEST_F(MedianContainerTest, threeUnorderedElemets)
{
    insert(2);
    insert(3);
    insert(1);
    check_median(2, 2);
}
TEST_F(MedianContainerTest, threeUnorderedElemets_differentCase)
{
    insert(2);
    insert(1);
    insert(3);
    check_median(2, 2);
}
TEST_F(MedianContainerTest, growingElements)
{
    for (int i = 0; i < 100; ++i)
    {
        insert(i);
        check_median(i / 2, i / 2 + i % 2);
    }
}
TEST_F(MedianContainerTest, elementsAreGettingSmaller)
{
    for (int i = 99; i >= 0; --i)
    {
        int diff = (99 - i);
        int expected = 99 - diff / 2;
        insert(i);
        if (i % 2 == 1)
            check_median(expected, expected);
        else
            check_median(expected - 1, expected);
    }
}
TEST_F(MedianContainerTest, randomOrder)
{
    int seed = 997;
    int N = 1223;
    std::vector<int> sofar;
    for (int i = 0; i < N; ++i)
    {
        int elem = ((i + 1) * seed) % N;
        insert(elem);
        sofar.push_back(elem);
        std::sort(sofar.begin(), sofar.end());
        int lower = i / 2;
        int upper = i / 2 + i % 2;
        check_median(sofar[lower], sofar[upper]);
    }
}
TEST_F(MedianContainerTest, insertSomeInRandomOrderFirstThanInsertOnlyBiggerThanAllOfThem)
{
    int N = 100;
    for (int i = 0; i < N; ++i)
        insert((i * 97) % N);
    check_median(49, 50);
    for (int j = 0; j < N; ++j)
    {
        insert(N + j);
        int lower = 50 + j / 2;
        int upper = 50 + j / 2 + j % 2;
        check_median(lower, upper);
    }
}
TEST_F(MedianContainerTest, insertSomeInRandomOrderFirstThanInsertOnlySmallerThanMost)
{
    int N = 100;
    for (int i = 0; i < N; ++i)
        insert((i * 97) % N);
    check_median(49, 50);
    for (int j = 0; j < N - 1; ++j)
    {
        insert(-j);
        int lower = 49 - j / 2 - j % 2;
        int upper = 49 - j / 2;
        ASSERT_NO_FATAL_FAILURE(check_median(lower, upper)) << j;
    }
    insert(-N + 1);
    ASSERT_NO_FATAL_FAILURE(check_median(0, 0));
}

TEST_F(MedianContainerTest, multipleGroupsOfSameElementsGrowing)
{
    int N = 10;
    std::vector<int> sofar;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            insert(i);
            sofar.push_back(i);
        }
        std::sort(sofar.begin(), sofar.end());
        int size = sofar.size();
        int lower = size / 2 - (size % 2 == 0);
        int upper = size / 2;
        check_median(sofar[lower], sofar[upper]);
    }
}

TEST_F(MedianContainerTest, multipleGroupsOfSameElementsGettingSmaller)
{
    int N = 10;
    std::vector<int> sofar;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            insert(N - i);
            sofar.push_back(N - i);
        }
        std::sort(sofar.begin(), sofar.end());
        int size = sofar.size();
        int lower = size / 2 - (size % 2 == 0);
        int upper = size / 2;
        check_median(sofar[lower], sofar[upper]);
    }
}

} //namespace

