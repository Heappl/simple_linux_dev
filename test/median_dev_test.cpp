#include "gtest/gtest.h"
#include "median_dev.h"
#include <algorithm>
#include <limits>
#include <cstring>

namespace
{
struct MedianDevTest : ::testing::Test
{
    median_dev dev = median_dev_init();

    ~MedianDevTest()
    {
        median_dev_release(dev);
    }

    void append_stream(std::string stream)
    {
        char* buff = median_dev_prepare_buff(dev, stream.size());
        std::memcpy(buff, stream.c_str(), stream.size());
        median_dev_append(dev, buff, stream.size());
    }

    void check_median(std::string expected)
    {
        auto str = median_dev_get(dev);
        std::string result(str.ptr, str.len);
        ASSERT_EQ(expected, result);

        str = median_dev_get(dev);
        ASSERT_EQ(0, str.len);
    }
};

TEST_F(MedianDevTest, simpleMedian)
{
    append_stream("1 2 3");
    check_median("2");
}

TEST_F(MedianDevTest, simpleMedianSplit)
{
    append_stream("1");
    check_median("1");

    append_stream("2 3");
    check_median("2");
}

TEST_F(MedianDevTest, someSimpleNegatives)
{
    append_stream("-1 -2 -3");
    check_median("-2");
}

} //namespace

