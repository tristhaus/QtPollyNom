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
    RandomDotgenerator generator(320, 41);

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
