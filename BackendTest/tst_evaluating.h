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

#ifndef TST_EVALUATING_H
#define TST_EVALUATING_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"
#include "../Backend/power.h"
#include "testexpressionbuilder.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, ComplexExpression01ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build01();
    double expected = 0.2;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression02ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build02();
    double expected = -0.00205809;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression03ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build03();
    double expected = 2.2;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression04ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build04();
    double expected = 1.21;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression05ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build05();
    double expected = 1.003182058;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression06ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build06();
    double expected = -1.9;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression07ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build07();
    double expected = 2.1;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

TEST(BackendTest, ComplexExpression08ShallEvaluateCorrectly)
{
    // Arrange
    auto reference = TestExpressionBuilder::Build08();
    double expected = 4.1;

    // Act
    auto optional = reference->Evaluate(0.1);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_DOUBLE_EQ(value, expected);
}

#endif // TST_EVALUATING_H
