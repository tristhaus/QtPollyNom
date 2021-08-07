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

#ifndef TST_SUBSETGENERATOR_H
#define TST_SUBSETGENERATOR_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <set>
#include "subsetgenerator.h"

using namespace testing;

TEST(InfastructureTest, SubsetGeneratorShallCreateCorrectSet)
{
    // Arrange
    SubsetGenerator generator(std::wstring(L"abc"));

    // Act
    std::set<std::wstring> result;
    std::wstring lastItem;
    for(int i = 0; i < 7; ++i)
    {
        ASSERT_TRUE(generator.HasNext());
        lastItem = generator.GetNext();
        result.insert(lastItem);
    }

    // Assert
    EXPECT_EQ(7, result.size());
    ASSERT_FALSE(generator.HasNext());
    EXPECT_EQ(1, result.count(L"a"));
    EXPECT_EQ(1, result.count(L"b"));
    EXPECT_EQ(1, result.count(L"c"));
    EXPECT_EQ(1, result.count(L"ab"));
    EXPECT_EQ(1, result.count(L"ac"));
    EXPECT_EQ(1, result.count(L"bc"));
    EXPECT_EQ(1, result.count(L"abc"));
    EXPECT_STREQ(L"abc", lastItem.c_str());
}

TEST(InfastructureTest, SubsetGeneratorShallThrowOnLongInput)
{
    // Arrange, Act, Assert
    try
    {
        SubsetGenerator generator(std::wstring(L"01234567890123456789012345678901234567890123456789012345678901234"));
        FAIL();
    }
    catch (std::exception&)
    {
        SUCCEED();
    }
}


#endif // TST_SUBSETGENERATOR_H
