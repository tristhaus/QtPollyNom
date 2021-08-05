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

#ifndef TST_MEMORYREPOSITORY_H
#define TST_MEMORYREPOSITORY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../TestHelper/memoryrepository.h"
#include "../Backend/game.h"
#include "../TestHelper/fixeddotgenerator.h"
#include <regex>

using namespace testing;
using namespace Backend;

TEST(BackendTest, MemoryRepositoryShallCorrectlyStoreGame)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>());

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"文字"),
        std::wstring(L""),
        std::wstring(L"")
    };

    MemoryRepository repo;
    std::wstring id = L"someId";

    // Act
    game.Update(exprStrings);

    auto result = repo.Save(game, id);

    std::wstring persisted;
    bool found = repo.TryGetByIdentifier(id, persisted);

    // Assert
    ASSERT_TRUE(result.first);
    ASSERT_TRUE(found);

    ASSERT_TRUE(persisted.length() > 0);
    std::wregex dataVersionRegex(LR"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, dataVersionRegex));
    std::wregex creationDateRegex(LR"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, creationDateRegex));
    std::wregex dotsRegex(LR"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, dotsRegex));
    std::wregex functionsRegex(LR"foo("functions":\["1/x","\(x\-3\.0\)\*\(x\+4\.0\)","\\u6587\\u5B57","",""\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(persisted, functionsRegex));
}

TEST(BackendTest, MemoryRepositoryShallCorrectlyRoundtripGame)
{
    // Arrange
    Game game(std::make_shared<FixedDotGenerator>(2));

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"文字"),
        std::wstring(L""),
        std::wstring(L"")
    };

    MemoryRepository repo;
    std::wstring id = L"someId";

    // Act
    game.Update(exprStrings);
    auto dots1 = game.GetDots();

    auto resultSave = repo.Save(game, id);

    game.Remake();

    auto dots2 = game.GetDots();
    auto functions2 = game.GetFunctions();

    auto resultLoad = repo.Load(id, game);

    auto dots3 = game.GetDots();
    auto functions3 = game.GetFunctions();

    std::wstring persisted;
    bool found = repo.TryGetByIdentifier(id, persisted);

    // Assert
    ASSERT_TRUE(resultSave.first);
    ASSERT_TRUE(resultLoad.first);
    ASSERT_TRUE(found);

    const double epsilon = 1e-6;

    ASSERT_EQ(dots1.size(), dots3.size());
    for(size_t i = 0; i < dots1.size(); ++i)
    {
        EXPECT_NEAR(dots1[i]->GetCoordinates().first, dots3[i]->GetCoordinates().first, epsilon);
        EXPECT_NEAR(dots1[i]->GetCoordinates().second, dots3[i]->GetCoordinates().second, epsilon);
        EXPECT_EQ(dots1[i]->IsGood(), dots3[i]->IsGood());
        EXPECT_NEAR(dots1[i]->GetRadius(), dots3[i]->GetRadius(), epsilon);
    }

    ASSERT_EQ(dots2.size(), dots3.size());
    for(size_t i = 0; i < dots2.size(); ++i)
    {
        EXPECT_NE(dots2[i]->GetCoordinates().first, dots3[i]->GetCoordinates().first);
        EXPECT_NE(dots2[i]->GetCoordinates().second, dots3[i]->GetCoordinates().second);
    }

    EXPECT_EQ(0, functions2.size());
    ASSERT_EQ(5, functions3.size());
    EXPECT_TRUE(functions3[0].compare(L"1/x") == 0);
    EXPECT_TRUE(functions3[1].compare(L"(x-3.0)*(x+4.0)") == 0);
    EXPECT_TRUE(functions3[2].compare(L"文字") == 0);
}

#endif // TST_MEMORYREPOSITORY_H
