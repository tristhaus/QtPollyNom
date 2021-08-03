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

#ifndef TST_GAME_H
#define TST_GAME_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/game.h"
#include "../TestHelper/fixeddotgenerator.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, GameShallUpdateCorrectlySingleStep)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string(""),
        std::string(""),
        std::string("")
    };

    // Act
    game.Update(exprStrings);
    auto dots = game.GetDots();
    auto graphs = game.GetGraphs();

    // Assert
    ASSERT_EQ(5, dots.size());
    EXPECT_TRUE(dots[0]->IsActive());
    EXPECT_TRUE(dots[1]->IsActive());
    EXPECT_TRUE(dots[2]->IsActive());
    EXPECT_FALSE(dots[3]->IsActive());
    EXPECT_FALSE(dots[4]->IsActive());

    EXPECT_EQ(3 + 1, game.GetScore());

    ASSERT_GT(graphs.size(), 0);
    ASSERT_GT(graphs[0].size(), 0);
    ASSERT_GT(graphs[0][0].first.size(), 0);
}

TEST(BackendTest, GameShallUpdateCorrectlyMultiStep)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings1 =
    {
        std::string("1/x"),
        std::string(""),
        std::string(""),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings2 =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string(""),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings3(exprStrings1);

    // Act 1
    game.Update(exprStrings1);
    auto dots1 = game.GetDots();

    // Assert 1
    ASSERT_EQ(5, dots1.size());
    EXPECT_TRUE(dots1[0]->IsActive());
    EXPECT_TRUE(dots1[1]->IsActive());
    EXPECT_FALSE(dots1[2]->IsActive());
    EXPECT_FALSE(dots1[3]->IsActive());
    EXPECT_FALSE(dots1[4]->IsActive());

    EXPECT_EQ(3, game.GetScore());

    // Act 2
    game.Update(exprStrings2);
    auto dots2 = game.GetDots();

    // Assert 2
    ASSERT_EQ(5, dots2.size());
    EXPECT_TRUE(dots2[0]->IsActive());
    EXPECT_TRUE(dots2[1]->IsActive());
    EXPECT_TRUE(dots2[2]->IsActive());
    EXPECT_FALSE(dots2[3]->IsActive());
    EXPECT_FALSE(dots2[4]->IsActive());

    EXPECT_EQ(3 + 1, game.GetScore());

    // Act 3
    game.Update(exprStrings3);
    auto dots3 = game.GetDots();

    // Assert 3
    ASSERT_EQ(5, dots3.size());
    EXPECT_TRUE(dots3[0]->IsActive());
    EXPECT_TRUE(dots3[1]->IsActive());
    EXPECT_FALSE(dots3[2]->IsActive());
    EXPECT_FALSE(dots3[3]->IsActive());
    EXPECT_FALSE(dots3[4]->IsActive());

    EXPECT_EQ(3, game.GetScore());
}

TEST(BackendTest, GameShallUpdateCorrectlyBadDotHit)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings1 =
    {
        std::string("1/x"),
        std::string(""),
        std::string(""),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings2 =
    {
        std::string("1/x"),
        std::string("5.05"),
        std::string(""),
        std::string(""),
        std::string("")
    };

    // Act 1
    game.Update(exprStrings1);
    auto dots1 = game.GetDots();

    // Assert 1
    ASSERT_EQ(5, dots1.size());
    EXPECT_TRUE(dots1[0]->IsActive());
    EXPECT_TRUE(dots1[1]->IsActive());
    EXPECT_FALSE(dots1[2]->IsActive());
    EXPECT_FALSE(dots1[3]->IsActive());
    EXPECT_FALSE(dots1[4]->IsActive());

    EXPECT_EQ(3, game.GetScore());

    // Act 2
    game.Update(exprStrings2);
    auto dots2 = game.GetDots();

    // Assert 2
    ASSERT_EQ(5, dots2.size());
    EXPECT_TRUE(dots2[0]->IsActive());
    EXPECT_TRUE(dots2[1]->IsActive());
    EXPECT_FALSE(dots2[2]->IsActive());
    EXPECT_FALSE(dots2[3]->IsActive());
    EXPECT_TRUE(dots2[4]->IsActive());

    EXPECT_LT(game.GetScore(), 0);
}

TEST(BackendTest, GameShallUpdateCorrectlyAndIgnoreOrder)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings1 =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string(""),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings2 =
    {
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings3 =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings4 =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::string("")
    };

    // Act 1
    game.Update(exprStrings1);
    auto dots1 = game.GetDots();

    // Assert 1
    ASSERT_EQ(5, dots1.size());
    EXPECT_TRUE(dots1[0]->IsActive());
    EXPECT_TRUE(dots1[1]->IsActive());
    EXPECT_TRUE(dots1[2]->IsActive());
    EXPECT_FALSE(dots1[3]->IsActive());
    EXPECT_FALSE(dots1[4]->IsActive());

    EXPECT_EQ(3 + 1, game.GetScore());

    // Act 2
    game.Update(exprStrings2);
    auto dots2 = game.GetDots();

    // Assert 2
    ASSERT_EQ(5, dots2.size());
    EXPECT_TRUE(dots2[0]->IsActive());
    EXPECT_TRUE(dots2[1]->IsActive());
    EXPECT_TRUE(dots2[2]->IsActive());
    EXPECT_FALSE(dots2[3]->IsActive());
    EXPECT_FALSE(dots2[4]->IsActive());

    EXPECT_EQ(7, game.GetScore());

    // Act 3
    game.Update(exprStrings3);
    auto dots3 = game.GetDots();

    // Assert 3
    ASSERT_EQ(5, dots3.size());
    EXPECT_TRUE(dots3[0]->IsActive());
    EXPECT_TRUE(dots3[1]->IsActive());
    EXPECT_TRUE(dots3[2]->IsActive());
    EXPECT_FALSE(dots3[3]->IsActive());
    EXPECT_FALSE(dots3[4]->IsActive());

    EXPECT_EQ(7, game.GetScore());

    // Act 4
    game.Update(exprStrings4);
    auto dots4 = game.GetDots();

    // Assert 4
    ASSERT_EQ(5, dots4.size());
    EXPECT_TRUE(dots4[0]->IsActive());
    EXPECT_TRUE(dots4[1]->IsActive());
    EXPECT_TRUE(dots4[2]->IsActive());
    EXPECT_TRUE(dots4[3]->IsActive());
    EXPECT_FALSE(dots4[4]->IsActive());

    EXPECT_EQ(15, game.GetScore());
}

TEST(BackendTest, GameShallRepeatedlyAndCorrectlyCalculateScore)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::string("")
    };

    // Act
    game.Update(exprStrings);
    auto dots = game.GetDots();
    auto score1 = game.GetScore();
    auto score2 = game.GetScore();

    // Assert
    ASSERT_EQ(5, dots.size());
    EXPECT_TRUE(dots[0]->IsActive());
    EXPECT_TRUE(dots[1]->IsActive());
    EXPECT_TRUE(dots[2]->IsActive());
    EXPECT_TRUE(dots[3]->IsActive());
    EXPECT_FALSE(dots[4]->IsActive());

    EXPECT_EQ(15, score1);
    EXPECT_EQ(15, score2);
}

TEST(BackendTest, GameShallCorrectlyCalculateScoreForPermutatedFunctions)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings1 =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::string("")
    };

    std::vector<std::string> exprStrings2 =
    {
        std::string("(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string(""),
        std::string("(x-3.0)*(x+4.0)"),
        std::string("1/x")
    };

    // Act
    game.Update(exprStrings1);
    auto score1 = game.GetScore();
    game.Update(exprStrings2);
    auto score2 = game.GetScore();

    // Assert
    EXPECT_EQ(15, score1);
    EXPECT_EQ(15, score2);
}

TEST(BackendTest, GameShallCorrectlyHandleOverlappingGraphs)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::string> exprStrings1 =
    {
        std::string("1/x"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("(x+4)*(x-1)*(x-4.95)"),
        std::string(""),
        std::string("")
    };

    std::vector<std::string> exprStrings2 =
    {
        std::string("1/x"),
        std::string("(x+8)*(x+4)*(x-1)"),
        std::string("(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::string(""),
        std::string("")
    };

    // Act 1
    game.Update(exprStrings1);
    auto dots1 = game.GetDots();
    auto score1 = game.GetScore();

    // Assert 1
    ASSERT_EQ(5, dots1.size());
    EXPECT_TRUE(dots1[0]->IsActive());
    EXPECT_TRUE(dots1[1]->IsActive());
    EXPECT_TRUE(dots1[2]->IsActive());
    EXPECT_TRUE(dots1[3]->IsActive());
    EXPECT_FALSE(dots1[4]->IsActive());

    EXPECT_EQ(7+1, score1);

    // Act 2
    game.Update(exprStrings2);
    auto dots2 = game.GetDots();
    auto score2 = game.GetScore();

    // Assert 2
    ASSERT_EQ(5, dots2.size());
    EXPECT_TRUE(dots2[0]->IsActive());
    EXPECT_TRUE(dots2[1]->IsActive());
    EXPECT_TRUE(dots2[2]->IsActive());
    EXPECT_TRUE(dots2[3]->IsActive());
    EXPECT_FALSE(dots2[4]->IsActive());

    EXPECT_EQ(15, score2);
}

TEST(BackendTest, GameRemakeShallCreateANewGame)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>(2));

    std::vector<std::string> exprStrings =
    {
        std::string("1/x"),
        std::string("(x-3.0)*(x+4.0)"),
        std::string(""),
        std::string(""),
        std::string("")
    };

    // Act
    game.Update(exprStrings);
    auto dots1 = game.GetDots();
    auto graphs1 = game.GetGraphs();
    game.Remake();
    auto dots2 = game.GetDots();
    auto graphs2 = game.GetGraphs();

    // Assert
    ASSERT_GT(graphs1.size(), 0);
    ASSERT_GT(graphs1[0].size(), 0);
    ASSERT_GT(graphs1[0][0].first.size(), 0);

    const double tolerance = 1e-9;
    for(unsigned int iter1 = 0; iter1 < dots1.size(); ++iter1)
    {
        auto & dot1 = dots1[iter1];
        for(unsigned int iter2 = 0; iter2 < dots2.size(); ++iter2)
        {
            auto & dot2 = dots2[iter2];

            auto evalFirst = abs(dot1->GetCoordinates().first - dot2->GetCoordinates().first) < tolerance;
            auto evalSecond = abs(dot1->GetCoordinates().second - dot2->GetCoordinates().second) < tolerance;
            auto evalRadius = abs(dot1->GetRadius() - dot2->GetRadius()) < tolerance;
            auto evalKind = dot1->IsGood() == dot2->IsGood();
            GTEST_ASSERT_FALSE(evalFirst && evalSecond && evalRadius && evalKind);
        }
    }

    ASSERT_EQ(graphs2.size(), 0);
    EXPECT_EQ(0, game.GetScore());
}

#endif // TST_GAME_H
