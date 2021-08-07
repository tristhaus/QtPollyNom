/*
 * This file is part of QtPollyNom.
 * 
 * QtPollyNom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * QtPollyNom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QtPollyNom.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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

    Sum sum({Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Minus, c)});

    // Act
    auto result1 = sum.Evaluate(0.0);
    auto result2 = sum.Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(-3.0, result1.value());
    EXPECT_DOUBLE_EQ(-7.5, result2.value());
}

TEST(BackendTest, SumShallPrintCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c = std::make_shared<Constant>(3.0);

    Sum sum({Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Minus, c)});

    // Act
    auto result1 = sum.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ(L"x-3.000000", result1.value().c_str());
}

#endif // TST_SUM_H
