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
#include "../Backend/gamepoc.h"
#include "fixeddotgenerator.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, GameShallUpdateCorrectlySingleStep)
{
    // Arrange
    GamePoc game(std::make_shared<FixedDotGenerator>());

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

    ASSERT_GT(graphs.size(), 0);
    ASSERT_GT(graphs[0].size(), 0);
    ASSERT_GT(graphs[0][0].first.size(), 0);
}

TEST(BackendTest, GameShallUpdateCorrectlyMultiStep)
{
    // Arrange
    GamePoc game(std::make_shared<FixedDotGenerator>());

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

    // Act 2
    game.Update(exprStrings2);
    auto dots2 = game.GetDots();

    // Assert 2
    ASSERT_EQ(5, dots2.size());
    EXPECT_TRUE(dots1[0]->IsActive());
    EXPECT_TRUE(dots1[1]->IsActive());
    EXPECT_TRUE(dots1[2]->IsActive());
    EXPECT_FALSE(dots1[3]->IsActive());
    EXPECT_FALSE(dots1[4]->IsActive());
}

#endif // TST_GAME_H
