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

#ifndef TST_PRINTINGTEST_H
#define TST_PRINTINGTEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/expression.h"
#include "testexpressionbuilder.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, Expression01ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build01();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "2.000000*x");
}

TEST(BackendTest, Expression02ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build02();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "2.000000*x^3.000000/(x-2.000000^x)");
}

TEST(BackendTest, Expression03ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build03();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "2.000000*(x+1.000000)");
}

TEST(BackendTest, Expression04ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build04();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "(x+1.000000)^2.000000");
}

TEST(BackendTest, Expression05ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build05();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "(x+1.000000)^(x/3.000000)");
}

TEST(BackendTest, Expression06ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build06();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "x-1.000000+2.000000-3.000000");
}

TEST(BackendTest, Expression07ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build07();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "x+1.000000-2.000000+3.000000");
}

TEST(BackendTest, Expression08ShallPrintCorrectly)
{
    // Arrange
    auto expression = TestExpressionBuilder::Build08();

    // Act
    auto optional = expression->Print();

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_STREQ(optional.value().c_str(), "x+1.000000-4.000000+7.000000");
}

#endif // TST_PRINTINGTEST_H
