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
#include "../TestHelper/memoryrepository.h"
#include "../TestHelper/doublehelper.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, GameShallUpdateCorrectlySingleStep)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
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

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings3(exprStrings1);

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

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"5.05"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
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

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings3 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings4 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L"")
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

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L"")
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

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L""),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"1/x")
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

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+8)*(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L""),
        std::wstring(L"")
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

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
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

TEST(BackendTest, GameShallAcceptDots)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::wstring> exprStrings1 =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::wstring> exprStrings2 =
    {
        std::wstring(L"-x"),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L""),
        std::wstring(L"")
    };

    std::vector<std::shared_ptr<Dot>> dots;
    dots.emplace_back(std::make_shared<Dot>(-3.0, 3.0));
    dots.emplace_back(std::make_shared<Dot>(2.0, -2.0));

    // Act
    game.Update(exprStrings1);
    auto score1 = game.GetScore();

    game.SetDots(dots);
    game.Update(exprStrings1);
    auto score2 = game.GetScore();

    auto storedDots = game.GetDots();

    game.Update(exprStrings2);
    auto score3 = game.GetScore();

    // Assert
    EXPECT_EQ(8, score1);
    EXPECT_EQ(0, score2);
    ASSERT_EQ(2, storedDots.size());
    EXPECT_EQ(-3.0, storedDots[0]->GetCoordinates().first);
    EXPECT_EQ(3.0, storedDots[0]->GetCoordinates().second);
    EXPECT_EQ(2.0, storedDots[1]->GetCoordinates().first);
    EXPECT_EQ(-2.0, storedDots[1]->GetCoordinates().second);
    EXPECT_EQ(3, score3);
}

TEST(BackendTest, GameShallSaveToRepository)
{
    // Arrange
    auto repository = std::make_shared<MemoryRepository>();
    Game game(std::make_shared<FixedDotGenerator>(), repository);
    std::wstring id = L"theIdentifier";

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    // Act
    game.Update(exprStrings);
    auto result = game.Save(id);
    std::wstring persisted;
    bool found = repository->TryGetByIdentifier(id, persisted);

    // Assert
    ASSERT_TRUE(found);
    ASSERT_TRUE(result.first);

    ASSERT_TRUE(persisted.length() > 0);
    std::wregex dataVersionRegex(LR"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, dataVersionRegex));
    std::wregex creationDateRegex(LR"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, creationDateRegex));
    std::wregex dotsRegex(LR"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, dotsRegex));
    std::wregex functionsRegex(LR"foo("functions":\["1/x","\(x\+8\)\*\(x\+4\)\*\(x-1\)","\(x\+4\)\*\(x-1\)\*\(x-4.95\)","",""\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, functionsRegex));
}

TEST(BackendTest, GameShallLoadFromRepository)
{
    // Arrange
    auto repository = std::make_shared<MemoryRepository>();
    Game game(std::make_shared<FixedDotGenerator>(2), repository);
    std::wstring id = L"theIdentifier";

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x+8)*(x+4)*(x-1)"),
        std::wstring(L"(x+4)*(x-1)*(x-4.95)"),
        std::wstring(L""),
        std::wstring(L"")
    };

    // Act
    game.Update(exprStrings);
    auto originalDots = game.GetDots();
    repository->Save(game, id);
    game.Remake();
    auto remadeDots = game.GetDots();
    auto result = game.Load(id);
    auto loadedDots = game.GetDots();

    // Assert
    ASSERT_TRUE(result.first);

    ASSERT_EQ(remadeDots.size(), loadedDots.size());

    // compare remade dots to dots from deserialization
    {
        bool onlyMatchesFound = true;
        for(unsigned short i= 0; i < remadeDots.size(); ++i)
        {
            auto & remadeDot = remadeDots[i];
            auto & loadedDot = loadedDots[i];

            if(!(AreClose(remadeDot->GetCoordinates().first, loadedDot ->GetCoordinates().first)
                 && AreClose(remadeDot->GetCoordinates().second, loadedDot ->GetCoordinates().second)
                 && remadeDot->IsGood() == loadedDot ->IsGood()
                 && remadeDot->GetRadius() == loadedDot ->GetRadius()))
            {
                onlyMatchesFound = false;
                break;
            }
        }

        ASSERT_FALSE(onlyMatchesFound);
    }

    ASSERT_EQ(originalDots.size(), loadedDots.size());

    // compare persisted dots to dots from deserialization
    {
        bool onlyMatchesFound = true;
        for(unsigned short i= 0; i < originalDots.size(); ++i)
        {
            auto & originalDot = originalDots[i];
            auto & loadedDot = loadedDots[i];

            if(!(AreClose(originalDot->GetCoordinates().first, loadedDot->GetCoordinates().first)
                 && AreClose(originalDot->GetCoordinates().second, loadedDot->GetCoordinates().second)
                 && originalDot->IsGood() == loadedDot->IsGood()
                 && originalDot->GetRadius() == loadedDot->GetRadius()))
            {
                onlyMatchesFound = false;
                break;
            }
        }

        ASSERT_TRUE(onlyMatchesFound);
    }
}

#endif // TST_GAME_H
