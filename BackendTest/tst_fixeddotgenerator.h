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

#ifndef TST_FIXEDDOTGENERATOR_H
#define TST_FIXEDDOTGENERATOR_H

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../TestHelper/fixeddotgenerator.h"
#include "../Backend/dot.h"

using namespace Backend;
using namespace testing;

TEST(BackendTest, FixedDotGeneratorWithPeriodOneShallAlwaysCreateSameSet)
{
    // Arrange
    const unsigned short period = 1;
    FixedDotGenerator generator(period);

    // Act
    auto instance1 = generator.Generate();
    auto instance2 = generator.Generate();

    // Assert
    const double tolerance = 1e-9;
    for(unsigned int iter = 0; iter < instance1.size(); ++iter)
    {
        auto & dot1 = instance1[iter];
        auto & dot2 = instance2[iter];

        auto evalFirst = abs(dot1->GetCoordinates().first - dot2->GetCoordinates().first) < tolerance;
        auto evalSecond = abs(dot1->GetCoordinates().second - dot2->GetCoordinates().second) < tolerance;
        auto evalRadius = abs(dot1->GetRadius() - dot2->GetRadius()) < tolerance;
        auto evalKind = dot1->IsGood() == dot2->IsGood();
        GTEST_ASSERT_TRUE(evalFirst && evalSecond && evalRadius && evalKind);
    }
}

TEST(BackendTest, FixedDotGeneratorWithDefaultArgumentsShallHavePeriodOne)
{
    // Arrange
    FixedDotGenerator generator;

    // Act
    auto instance1 = generator.Generate();
    auto instance2 = generator.Generate();

    // Assert
    const double tolerance = 1e-9;
    for(unsigned int iter = 0; iter < instance1.size(); ++iter)
    {
        auto & dot1 = instance1[iter];
        auto & dot2 = instance2[iter];

        auto evalFirst = abs(dot1->GetCoordinates().first - dot2->GetCoordinates().first) < tolerance;
        auto evalSecond = abs(dot1->GetCoordinates().second - dot2->GetCoordinates().second) < tolerance;
        auto evalRadius = abs(dot1->GetRadius() - dot2->GetRadius()) < tolerance;
        auto evalKind = dot1->IsGood() == dot2->IsGood();
        GTEST_ASSERT_TRUE(evalFirst && evalSecond && evalRadius && evalKind);
    }
}

TEST(BackendTest, FixedDotGeneratorWithPeriodTwoShallCreateTwoDifferentSets)
{
    // Arrange
    const unsigned short period = 2;
    FixedDotGenerator generator(period);

    // Act
    auto instance1 = generator.Generate();
    auto instance2 = generator.Generate();
    auto instance3 = generator.Generate();

    // Assert
    const double tolerance = 1e-9;
    for(unsigned int iter1 = 0; iter1 < instance1.size(); ++iter1)
    {
        auto & dot1 = instance1[iter1];
        for(unsigned int iter2 = 0; iter2 < instance2.size(); ++iter2)
        {
            auto & dot2 = instance2[iter2];

            auto evalFirst = abs(dot1->GetCoordinates().first - dot2->GetCoordinates().first) < tolerance;
            auto evalSecond = abs(dot1->GetCoordinates().second - dot2->GetCoordinates().second) < tolerance;
            auto evalRadius = abs(dot1->GetRadius() - dot2->GetRadius()) < tolerance;
            auto evalKind = dot1->IsGood() == dot2->IsGood();
            GTEST_ASSERT_FALSE(evalFirst && evalSecond && evalRadius && evalKind);
        }
    }

    for(unsigned int iter = 0; iter < instance1.size(); ++iter)
    {
        auto & dot1 = instance1[iter];
        auto & dot3 = instance3[iter];

        auto evalFirst = abs(dot1->GetCoordinates().first - dot3->GetCoordinates().first) < tolerance;
        auto evalSecond = abs(dot1->GetCoordinates().second - dot3->GetCoordinates().second) < tolerance;
        auto evalRadius = abs(dot1->GetRadius() - dot3->GetRadius()) < tolerance;
        auto evalKind = dot1->IsGood() == dot3->IsGood();
        GTEST_ASSERT_TRUE(evalFirst && evalSecond && evalRadius && evalKind);
    }
}

#endif // TST_FIXEDDOTGENERATOR_H
