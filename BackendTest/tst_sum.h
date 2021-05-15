#ifndef TST_SUM_H
#define TST_SUM_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/sum.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, SumShallEvaluateCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c = std::make_shared<Constant>(3.0);

    std::vector<Sum::Summand> summands;
    summands.push_back(Sum::Summand(Sum::Sign::Plus, x));
    summands.push_back(Sum::Summand(Sum::Sign::Minus, c));

    Sum sum(summands);

    // Act
    auto result1 = sum.Evaluate(0.0);
    auto result2 = sum.Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(result1.value(), -3.0);
    EXPECT_DOUBLE_EQ(result2.value(), -7.5);
}

TEST(BackendTest, SumShallPrintCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c = std::make_shared<Constant>(3.0);

    std::vector<Sum::Summand> summands;
    summands.push_back(Sum::Summand(Sum::Sign::Plus, x));
    summands.push_back(Sum::Summand(Sum::Sign::Minus, c));

    Sum sum(summands);

    // Act
    auto result1 = sum.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ(result1.value().c_str(), "x-3.000000");
}

#endif // TST_SUM_H
