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

#ifndef TST_BASEX_H
#define TST_BASEX_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/basex.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, BaseXShallEvaluateCorrectly)
{
    // Arrange
    BaseX x;

    // Act
    auto result1 = x.Evaluate(0.0);
    auto result2 = x.Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(0.0, result1.value());
    EXPECT_DOUBLE_EQ(-4.5, result2.value());
}

TEST(BackendTest, BaseXShallPrintCorrectly)
{
    // Arrange
    BaseX x;

    // Act
    auto result1 = x.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ("x", result1.value().c_str());
}

#endif // TST_BASEX_H
