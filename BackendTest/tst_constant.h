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

#ifndef TST_CONSTANT_H
#define TST_CONSTANT_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, ConstantShallEvaluateCorrectly)
{
    // Arrange
    Constant constant(4.8);

    // Act
    auto result1 = constant.Evaluate(0.0);

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_DOUBLE_EQ(4.8, result1.value());
}

TEST(BackendTest, ConstantShallPrintCorrectly)
{
    // Arrange
    Constant constant(-4.8);

    // Act
    auto result1 = constant.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ("-4.800000", result1.value().c_str());
}

#endif // TST_CONSTANT_H
