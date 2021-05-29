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

#ifndef TST_RANDOMDOTGENERATOR_H
#define TST_RANDOMDOTGENERATOR_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/randomdotgenerator.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, RandomDotGeneratorShallCreateDotsInsideCertainRange)
{
    // Arrange
    RandomDotGenerator generator(320, 41);

    // Act
    auto dots = generator.Generate();

    // Assert
    auto dotsIt = dots.begin();
    auto dotsEnd = dots.end();

    for (; dotsIt != dotsEnd; ++dotsIt)
    {
        EXPECT_LE((*dotsIt)->GetCoordinates().first, 9.25);
        EXPECT_GE((*dotsIt)->GetCoordinates().first, -9.25);
        EXPECT_LE((*dotsIt)->GetCoordinates().second, 9.25);
        EXPECT_GE((*dotsIt)->GetCoordinates().second, -9.25);
    }
}

#endif // TST_RANDOMDOTGENERATOR_H
