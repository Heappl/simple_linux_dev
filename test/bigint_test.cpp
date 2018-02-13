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
        bigint_destroy(xb);
    }
};

TEST_F(BigintTest, creatingFromIntAndComparing)
{
    bigint x = bigint_create(10, alloc);
    bigint y = bigint_create(11, alloc);
    bigint z = bigint_create(13, alloc);
    bigint v = bigint_create(9, alloc);
    bigint k = bigint_create(-1, alloc);
    bigint l = bigint_create(0, alloc);
    bigint m = bigint_create(-9, alloc);
    //compare_multiple({m, k, l, v, x, y, z});
    compare_multiple({l, v, x, y, z});
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
    bigint_mult_i(xb, multi);
    checkToString(xb, std::to_string(xi));
}

TEST_F(BigintTest, toStringGoogol)
{
    bigint x = bigint_create(10, alloc);
    std::string expected = "10";
    for (int i = 0; i < 100; ++i)
    {
        bigint_mult_i(x, 10);
        expected += "0";
    }
    checkToString(x, expected);
}

TEST_F(BigintTest, toStringConsecutiveMult)
{
    bigint x = bigint_create(11, alloc);
    for (int i = 101; i < 121; ++i)
        bigint_mult_i(x, i);
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
        bigint_mult_i(x, i);
        ASSERT_NO_FATAL_FAILURE(compare(y, x)) << i;
        bigint_mult_i(y, i);
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
            bigint_add_u(x, j);
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
    bigint_add_u(x, 12322);
    checkToString(x, "123124912859851606");
}

TEST_F(BigintTest, add1ToALotsOf9s)
{
    std::string first = "99999999999999999999999999999999";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint_add_u(x, 1);
    checkToString(x, "100000000000000000000000000000000");
}

TEST_F(BigintTest, multiplyBigAndDivWithIndependent)
{
    std::string first = "123405054321123405054321123405054321123405054321";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint_mult_i(x, 2);
    checkToString(x, "246810108642246810108642246810108642246810108642");
    bigint_add_u(x, 1);
    ASSERT_EQ(1, bigint_div_u(x, 2));
    checkToString(x, first);
}

TEST_F(BigintTest, divSmallWithoutReminder)
{
    for (int i = 1; i < 128; ++i)
    {
        for (int j = 1; j < 128; ++j)
        {
            bigint x = bigint_create(i * j, alloc);
            ASSERT_EQ(0, bigint_div_u(x, i));
            checkToString(x, std::to_string(j));
            bigint_destroy(x);
        }
    }
}

TEST_F(BigintTest, addSmallBigInts)
{
    for (int i = 0; i < 256; ++i)
    { 
        bigint x = bigint_create(i, alloc);
        uint64_t aux = i;
        for (int j = 0; j < 256; ++j)
        {
            bigint y = bigint_create(j, alloc);
            bigint_add(x, y);
            aux += j;
            checkToString(x, std::to_string(aux));
            bigint_destroy(y);
        }
        bigint_destroy(x);
    }
}

TEST_F(BigintTest, subSmallBigInts)
{
    //for (int i = -256; i < 256; ++i)
    //{ 
        //bigint x = bigint_create(i, alloc);
        //int64_t aux = i;
        //for (int j = -256; j < 256; ++j)
        //{
            //bigint y = bigint_create(-j, alloc);
            //bigint_add(x, y);
            //aux -= j;
            //ASSERT_NO_FATAL_FAILURE(checkToString(x, std::to_string(aux)))
                //<< (aux + j) << " " << j << " " << aux;
            //bigint_destroy(y);
        //}
        //bigint_destroy(x);
    //}
}

TEST_F(BigintTest, add64BitBigInts)
{
    uint64_t aux = 132131231;
    bigint x = bigint_create(aux, alloc);
    int mult = 999213999;
    for (int i = 1; i < 123123; i += 12)
    {
        aux += i * uint64_t(mult);
        bigint y = bigint_create(i, alloc);
        bigint_mult_i(y, mult);
        bigint_add(x, y);
        ASSERT_NO_FATAL_FAILURE(checkToString(x, std::to_string(aux)));
        bigint_destroy(y);
    }
    bigint_destroy(x);
}

TEST_F(BigintTest, addPowerTwos)
{
    std::string first = "2475880078570760549798248448";
    std::string second = "302231454903657293676544";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint y = bigint_fromstr(second.c_str(), second.size(), alloc);
    ASSERT_NO_FATAL_FAILURE(checkToString(x, first));
    ASSERT_NO_FATAL_FAILURE(checkToString(y, second));
    bigint_add(x, y);
    ASSERT_NO_FATAL_FAILURE(checkToString(x, "2476182310025664207091924992"));
    bigint_add(y, x);
    ASSERT_NO_FATAL_FAILURE(checkToString(y, "2476484541480567864385601536"));
}
TEST_F(BigintTest, indepentedBigInts)
{
    std::string first =  "1234567890123456789012345678901234567890";
    std::string second = "8765432109876543210987654321098765432109";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint y = bigint_fromstr(second.c_str(), second.size(), alloc);
    bigint_add(x, y);
    checkToString(x, "9999999999999999999999999999999999999999");
}

TEST_F(BigintTest, addToBigInts)
{
    std::string first = "123405054321123405054321123405054321123405054321";
    std::string second = "99999999999999999999999999999999";
    bigint x = bigint_fromstr(first.c_str(), first.size(), alloc);
    bigint y = bigint_fromstr(second.c_str(), second.size(), alloc);
    checkToString(x, first);
    checkToString(y, second);
    bigint_add(y, x);
    checkToString(y, "123405054321123505054321123405054321123405054320");
    bigint_destroy(y);
    bigint_destroy(x);
}

} //namespace

