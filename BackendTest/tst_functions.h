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

#ifndef TST_FUNCTIONS_H
#define TST_FUNCTIONS_H

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/expression.h"
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/functions.h"

using namespace Backend;
using namespace testing;

#define _USE_MATH_DEFINES
#include <math.h>

TEST(BackendTest, SineShallEvaluateCorrectly)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto sine = std::make_shared<Sine>(baseX);

    // Act
    auto optional = sine->Evaluate(M_PI/6.0);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_NEAR(value, 0.5, 1e-6);
}

TEST(BackendTest, CosineShallEvaluateCorrectly)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto cosine = std::make_shared<Cosine>(baseX);

    // Act
    auto optional = cosine->Evaluate(M_PI/3.0);
    double value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_NEAR(value, 0.5, 1e-6);
}

#endif // TST_FUNCTIONS_H
