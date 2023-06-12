#include "interfaces.hpp"

#include "gtest/gtest.h"

class TestInterface : public testing::Test
{};

TEST_F(TestInterface, testInterface_1)
{
    EXPECT_EQ(getVectorSize(1), 3);
    EXPECT_NE(getVectorSize(1), 4);
}

TEST_F(TestInterface, testInterface_2)
{
    EXPECT_EQ(getModeString(ModeList::starting), "Starting");
}

TEST_F(TestInterface, testInterface_3)
{
    EXPECT_EQ(getModeEnum("Working"), ModeList::working);
}
