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

#ifndef TST_DISKREPOSITORY_H
#define TST_DISKREPOSITORY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/game.h"
#include "../Backend/diskrepository.h"
#include "../TestHelper/fixeddotgenerator.h"
#include <filesystem>
#include <fstream>

using namespace testing;
using namespace Backend;

#if defined(_USE_LONG_TEST)
TEST(BackendTest, DiskRepositoryShallCorrectlyRoundtripGame)
{
    // Arrange
    auto tempFile = std::filesystem::temp_directory_path() / std::filesystem::path(L"qtpollynom.testing.文字.temp.file");
    Game game(std::make_shared<FixedDotGenerator>(2));

    std::vector<std::wstring> exprStrings =
    {
        std::wstring(L"1/x"),
        std::wstring(L"(x-3.0)*(x+4.0)"),
        std::wstring(L"文字"),
        std::wstring(L""),
        std::wstring(L"")
    };

    DiskRepository repo;
    std::wstring id = tempFile.wstring();

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

    // Assert
    ASSERT_TRUE(resultSave.first);
    ASSERT_TRUE(resultLoad.first);

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

    ASSERT_TRUE(std::filesystem::exists(tempFile));
    ASSERT_TRUE(std::filesystem::remove(tempFile));
    ASSERT_FALSE(std::filesystem::exists(tempFile));
}
#endif // _USE_LONG_TEST

#endif // TST_DISKREPOSITORY_H
