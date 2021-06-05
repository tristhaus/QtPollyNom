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

#ifndef TST_FUNDAMENTAL_H
#define TST_FUNDAMENTAL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <cmath>

using namespace testing;

TEST(BackendTest, AssumedMathErrorHandlingShallBeInPlace)
{
    // Assert
    ASSERT_TRUE(math_errhandling & MATH_ERREXCEPT);
}

#endif // TST_FUNDAMENTAL_H
