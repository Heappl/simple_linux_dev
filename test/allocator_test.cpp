#include "gtest/gtest.h"
#include "allocator.h"

namespace
{
struct SysAllocMock
{
    MOCK_METHOD0(void* allocate());
    MOCK_METHOD1(void deallocate(void*));
};

struct AllocatorTest : ::testing::Test
{
    StrictMock<SysAllocMock> mock;
    allocator_info mock_ctx = {
        (void*)*mock,
        [](void* ctx){ return ((SysAllocMock*)ctx)->allocate(); },
        [](void* ctx, void* ptr){ return ((SysAllocMock*)ctx)->allocate(ptr); },
        sizeof(int) * 10
    };
};

TEST_F(AllocatorTest, SingleAllocAndFree) {
    int buffer[2];
    EXPECT_CALL(mock, allocate()).WillOnce(Return((void*)&buffer));
    ASSERT_EQ(&buffer, allocator_alloc(mock_ctx));
    allocator_free(mock_ctx, &buffer);
    ASSERT_EQ(&buffer, allocator_alloc(mock_ctx));
    allocator_free(mock_ctx, buffer);
}

} //namespace

