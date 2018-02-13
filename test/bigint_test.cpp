#include "gtest/gtest.h"
#include "bigint.h"
#include <algorithm>
#include <limits>

namespace
{

struct BigintTest : ::testing::Test
{
    allocator_info alloc = {
        nullptr,
        [](void*, unsigned size){ return malloc(size); },
        [](void*, void* ptr){ free(ptr); }
    };

    void compare(bigint x, bigint y)
    {
        ASSERT_TRUE(bigint_less(x, y));
        ASSERT_FALSE(bigint_less(y, x));
        ASSERT_TRUE(bigint_greater(y, x));
        ASSERT_FALSE(bigint_greater(x, y));
    }

    void compare_multiple(std::vector<bigint> ordered)
    {
        for (int i = 0; i < ordered.size(); ++i)
        {
            ASSERT_FALSE(bigint_less(ordered[i], ordered[i]));
            ASSERT_FALSE(bigint_greater(ordered[i], ordered[i]));
            for (int j = i + 1; j < ordered.size(); ++j)
                compare(ordered[i], ordered[j]);
        }
    }

    void checkToString(bigint x, std::string expected)
    {
        std::vector<char> buffer(expected.size() + 1, 'A');
        bigint_tostr(x, &buffer.front(), buffer.size());
        ASSERT_STREQ(expected.c_str(), &buffer.front());
    }

    template <typename T>
    void checkFromString(T x)
    {
        std::string from = std::to_string(x);
        bigint xb = bigint_fromstr(from.c_str(), from.size(), alloc);
        int xi = bigint_toint(xb);
        if (x < std::numeric_limits<int>::max())
            ASSERT_EQ(x, xi);
        char buffer[10000];
        bigint_tostr(xb, buffer, sizeof(buffer));
        ASSERT_STREQ(from.c_str(), buffer);
    }
};

TEST_F(BigintTest, creatingFromIntAndComparing)
{
    bigint x = bigint_create(10, alloc);
    bigint y = bigint_create(11, alloc);
    bigint z = bigint_create(13, alloc);
    bigint v = bigint_create(9, alloc);
    compare_multiple({v, x, y, z});
}

TEST_F(BigintTest, toStringForSmall)
{
    checkToString(bigint_create(10, alloc), "10");
    checkToString(bigint_create(121312, alloc), "121312");
    checkToString(bigint_create(0, alloc), "0");
    checkToString(bigint_create(-1231, alloc), "-1231");
    checkToString(bigint_create(999, alloc), "999");
    checkToString(bigint_create(-110000, alloc), "-110000");
}

TEST_F(BigintTest, toString64BitNumber)
{
    uint64_t xi = 1312333101;
    bigint xb = bigint_create(xi, alloc);
    int multi = 41239;
    xi *= multi;
    bigint_mult(xb, multi);
    checkToString(xb, std::to_string(xi));
}

TEST_F(BigintTest, toStringGoogol)
{
    bigint x = bigint_create(10, alloc);
    std::string expected = "10";
    for (int i = 0; i < 100; ++i)
    {
        bigint_mult(x, 10);
        expected += "0";
    }
    checkToString(x, expected);
}

TEST_F(BigintTest, toStringConsecutiveMult)
{
    bigint x = bigint_create(11, alloc);
    for (int i = 101; i < 121; ++i)
        bigint_mult(x, i);
    std::string expected = "788465831362662701050293791632835543040000";
    checkToString(x, expected);
}

TEST_F(BigintTest, multiplicationWithComparison)
{
    bigint x = bigint_create(10, alloc);
    bigint y = bigint_create(11, alloc);
    compare(x, y);

    for (int i = 101; i < 1099; ++i)
    {
        bigint_mult(x, i);
        ASSERT_NO_FATAL_FAILURE(compare(y, x)) << i;
        bigint_mult(y, i);
        ASSERT_NO_FATAL_FAILURE(compare(x, y)) << i;
    }
}

TEST_F(BigintTest, additionForSmallNumbers)
{
    for (int i = 0; i < 1012; i += 5)
    {
        uint64_t expected = i;
        bigint x = bigint_create(i, alloc);
        for (int j = 9; j < 546; j += 2)
        {
            bigint_add(x, j);
            expected += j;
            checkToString(x, std::to_string(expected));
        }
    }
}

TEST_F(BigintTest, fromString64BitValues)
{
    for (int i = 1; i < 100; ++i)
    {
        ASSERT_NO_FATAL_FAILURE(checkFromString(1231122321llu * i));
        ASSERT_NO_FATAL_FAILURE(checkFromString(889llu * i + (1llu << 33) - 3));
        ASSERT_NO_FATAL_FAILURE(checkFromString((1llu << 56) - i));
    }
}

TEST_F(BigintTest, fromStringSmallValues)
{
    for (int i = 1; i < 100; ++i)
    {
        ASSERT_NO_FATAL_FAILURE(checkFromString(123112 + i));
        ASSERT_NO_FATAL_FAILURE(checkFromString(889 * i));
        ASSERT_NO_FATAL_FAILURE(checkFromString(i));
    }
}

TEST_F(BigintTest, additionForBigNumberAndSmall)
{
    std::string first = "123124912859839284";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint_add(x, 12322);
    checkToString(x, "123124912859851606");
}

TEST_F(BigintTest, add1ToALotsOf9s)
{
    std::string first = "99999999999999999999999999999999";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint_add(x, 1);
    checkToString(x, "100000000000000000000000000000000");
}

TEST_F(BigintTest, multiplyBigWithIndependent)
{
    std::string first = "123405054321123405054321123405054321123405054321";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint_mult(x, 2);
    checkToString(x, "246810108642246810108642246810108642246810108642");
}

} //namespace

