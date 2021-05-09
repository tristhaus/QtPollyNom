#ifndef TST_CONSTANT_H
#define TST_CONSTANT_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

#include "../Backend/constant.h"

using namespace Backend;

TEST(BackendTest, ConstantShallEvaluateCorrectly)
{
    // Arrange
    Constant constant(4.8);

    // Act
    auto result1 = constant.Evaluate(0.0);

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_DOUBLE_EQ(result1.value(), 4.8);
}

TEST(BackendTest, ConstantShallPrintCorrectly)
{
    // Arrange
    Constant constant(-4.8);

    // Act
    auto result1 = constant.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ(result1.value().c_str(), "-4.800000");
}

#endif // TST_CONSTANT_H
