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

#ifndef TST_DESERIALIZER_H
#define TST_DESERIALIZER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <regex>
#include "../Backend/deserializer.h"
#include "../Backend/game.h"
#include "../TestHelper/fixeddotgenerator.h"
#include "../TestHelper/doublehelper.h"

#include <sstream>

using namespace testing;
using namespace Backend;

TEST(BackendTest, SerializationOfGame1WithoutFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::wstringstream ss;
    DeSerializer ds;
    Game game(std::make_shared<FixedDotGenerator>());

    // Act
    ds.Serialize(game, ss);

    // Assert
    auto result = ss.str();

    ASSERT_TRUE(result.length() > 0);
    std::wregex dataVersionRegex(LR"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dataVersionRegex));
    std::wregex creationDateRegex(LR"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, creationDateRegex));
    std::wregex dotsRegex(LR"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dotsRegex));
    std::wregex functionsRegex(LR"foo("functions":\[\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, functionsRegex));
}

TEST(BackendTest, SerializationOfGame1WithFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::wstringstream ss;
    DeSerializer ds;
    Game game(std::make_shared<FixedDotGenerator>());
    game.Update(std::vector<std::wstring> {L"sin(x)", L"x^2", L"abc"});

    // Act
    ds.Serialize(game, ss);

    // Assert
    auto result = ss.str();

    ASSERT_TRUE(result.length() > 0);
    std::wregex dataVersionRegex(LR"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dataVersionRegex));
    std::wregex creationDateRegex(LR"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, creationDateRegex));
    std::wregex dotsRegex(LR"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dotsRegex));
    std::wregex functionsRegex(LR"foo("functions":\["sin\(x\)","x\^2","abc"\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, functionsRegex));
}

struct TestDeserializationErrorResult
{
    std::wstring testname;
    std::wstring json;
    friend std::wostream& operator<<(std::wostream& wos, const TestDeserializationErrorResult& obj)
    {
        return wos
                << L"testname: " << obj.testname
                << L" json: " << obj.json;
    }
};

class DeserializationErrorTest : public testing::TestWithParam<TestDeserializationErrorResult>
{
};

INSTANTIATE_TEST_SUITE_P(BackendTest, DeserializationErrorTest, // clazy:exclude=non-pod-global-static
    testing::Values(
    TestDeserializationErrorResult{L"NotObject", LR"foo([])foo"},
    TestDeserializationErrorResult{L"EmptyObject", LR"foo({})foo"},
    TestDeserializationErrorResult{L"DataVersionNotCorrect1", LR"foo({"dataVersion":0.0})foo"},
    TestDeserializationErrorResult{L"DataVersionNotCorrect2", LR"foo({"dataVersion":"a"})foo"},
    TestDeserializationErrorResult{L"NoDotsMember", LR"foo({"dataVersion":"1"})foo"},
    TestDeserializationErrorResult{L"InvalidDot1", LR"foo({"dataVersion":"1","dots":[{"x":"a"}]})foo"},
    TestDeserializationErrorResult{L"InvalidDot2", LR"foo({"dataVersion":"1","dots":[{"x":0.25,"y":0.17,"radius":0.33,"kind":true}]})foo"},
    TestDeserializationErrorResult{L"InvalidDot3", LR"foo({"dataVersion":"1","dots":[{"x":0.25,"y":0.17,"radius":0.33,"kind":"bla"}]})foo"},
    TestDeserializationErrorResult{L"InvalidDot4", LR"foo({"dataVersion":"1","dots":[{"x":0.25,"y":0.17,"radius":0.33,"kind":"good"},{"x":-0.25,"y":-0.17,"radius":0.66,"kind":"bla"}]})foo"},
    TestDeserializationErrorResult{L"InvalidDot5", LR"foo({"dataVersion":"1","dots":[{"x":0.25,"y":0.17,"radius":-0.33,"kind":"good"}]})foo"},
    TestDeserializationErrorResult{L"NoFunctionsMember", LR"foo({"dataVersion":"1","dots":[{"x":0.25,"y":0.17,"radius":0.33,"kind":"good"}]})foo"},
    TestDeserializationErrorResult{L"InvalidFunction", LR"foo({"dataVersion":"1","functions":["sin(x)",true,"x^2"],"dots":[{"x":0.25,"y":0.17,"radius":0.33,"kind":"good"}]})foo"}
));

TEST_P(DeserializationErrorTest, GivenBadJsonDeserializationShouldGiveError)
{
    // Arrange
    TestDeserializationErrorResult tder = GetParam();
    std::wstringstream ss;
    ss << tder.json;
    ss.seekg(0, std::ios::beg);

    DeSerializer ds;
    Game game(std::make_shared<FixedDotGenerator>());

    // Act
    auto result = ds.Deserialize(ss, game);

    // Assert
    ASSERT_FALSE(result.first);
    ASSERT_LT(0, result.second.length());
}

TEST(BackendTest, DeserializationOfGame1WithFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::wstringstream ss;
    ss << LR"foo({"dataVersion":"1","creationDate":"2021-08-02T19:41:09Z\u0000","dots":[{"x":1.0,"y":1.0,"radius":0.25,"kind":"good"},{"x":-8.0,"y":-0.25,"radius":0.25,"kind":"good"},{"x":-4.0,"y":0.35,"radius":0.25,"kind":"good"},{"x":5.0,"y":-5.0,"radius":0.25,"kind":"good"},{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"}],"functions":["1/x","(x-3.0)*(x+4.0)","(x+8)*(x+4)*(x-1)"]})foo";
    ss.seekg(0, std::ios::beg);

    DeSerializer ds;
    Game game(std::make_shared<RandomDotGenerator>(6, 1));

    // Act
    auto randomDots = game.GetDots();
    auto noFunctions = game.GetFunctions();

    auto result = ds.Deserialize(ss, game);

    auto dotsFromPersistence = game.GetDots();
    auto functions = game.GetFunctions();
    auto score = game.GetScore();

    // Assert
    ASSERT_TRUE(result.first);

    if(randomDots.size() == dotsFromPersistence.size())
    {
        bool onlyMatchesFound = true;
        for(unsigned short i= 0; i < randomDots.size(); ++i)
        {
            auto & randomDot = randomDots[i];
            auto & dotFromPersistence = dotsFromPersistence[i];

            if(!(AreClose(randomDot->GetCoordinates().first, dotFromPersistence->GetCoordinates().first)
                 && AreClose(randomDot->GetCoordinates().second, dotFromPersistence->GetCoordinates().second)
                 && randomDot->IsGood() == dotFromPersistence->IsGood()
                 && randomDot->GetRadius() == dotFromPersistence->GetRadius()))
            {
                onlyMatchesFound = false;
                break;
            }
        }

        ASSERT_FALSE(onlyMatchesFound);
    }
    else
    {
        ASSERT_TRUE(true);
    }

    ASSERT_EQ(0, noFunctions.size());

    ASSERT_EQ(5, dotsFromPersistence.size());
    EXPECT_EQ(-8.0, dotsFromPersistence[1]->GetCoordinates().first);
    EXPECT_EQ(-0.25, dotsFromPersistence[1]->GetCoordinates().second);
    EXPECT_TRUE(dotsFromPersistence[1]->IsGood());
    EXPECT_EQ(0.25, dotsFromPersistence[1]->GetRadius());
    EXPECT_EQ(2.5, dotsFromPersistence[4]->GetCoordinates().first);
    EXPECT_EQ(5.0, dotsFromPersistence[4]->GetCoordinates().second);
    EXPECT_FALSE(dotsFromPersistence[4]->IsGood());
    EXPECT_EQ(0.25, dotsFromPersistence[4]->GetRadius());

    ASSERT_EQ(5, functions.size());
    EXPECT_STREQ(L"1/x", functions[0].c_str());
    EXPECT_STREQ(L"(x-3.0)*(x+4.0)", functions[1].c_str());
    EXPECT_STREQ(L"(x+8)*(x+4)*(x-1)", functions[2].c_str());
    EXPECT_STREQ(L"", functions[3].c_str());
    EXPECT_STREQ(L"", functions[4].c_str());

    EXPECT_EQ(7, score);
}

TEST(BackendTest, DeserializationOfGameWithoutDotsOrFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::wstringstream ss;
    ss << LR"foo({"dataVersion":"1","creationDate":"2021-08-02T19:41:09Z\u0000","dots":[],"functions":["","","","",""]})foo";
    ss.seekg(0, std::ios::beg);

    DeSerializer ds;
    Game game(std::make_shared<RandomDotGenerator>(6, 1));

    // Act
    auto randomDots = game.GetDots();
    auto noFunctions = game.GetFunctions();

    auto result = ds.Deserialize(ss, game);

    auto dotsFromPersistence = game.GetDots();
    auto functions = game.GetFunctions();
    auto score = game.GetScore();

    // Assert
    ASSERT_TRUE(result.first);

    ASSERT_NE(randomDots.size(), dotsFromPersistence.size());
    ASSERT_EQ(0, noFunctions.size());

    ASSERT_EQ(0, dotsFromPersistence.size());

    ASSERT_EQ(5, functions.size());
    EXPECT_STREQ(L"", functions[0].c_str());
    EXPECT_STREQ(L"", functions[1].c_str());
    EXPECT_STREQ(L"", functions[2].c_str());
    EXPECT_STREQ(L"", functions[3].c_str());
    EXPECT_STREQ(L"", functions[4].c_str());

    EXPECT_EQ(0, score);
}

TEST(BackendTest, DeserializationSerializationRoundtripShouldWorkCorrectly)
{
    // Arrange
    DeSerializer ds;
    Game game(std::make_shared<RandomDotGenerator>(6, 1));

    std::wstringstream ss;

    // Act
    auto persistedDots = game.GetDots();
    game.Update(std::vector<std::wstring> {L"tan(x)", L"x^3"});

    ds.Serialize(game, ss);
    ss.seekg(0, std::ios::beg);

    game.Remake();

    auto randomDots = game.GetDots();
    auto noFunctions = game.GetFunctions();

    auto result = ds.Deserialize(ss, game);

    auto dotsFromPersistence = game.GetDots();
    auto functionsFromPersistence = game.GetFunctions();

    // Assert
    ASSERT_TRUE(result.first);

    ASSERT_EQ(randomDots.size(), dotsFromPersistence.size());
    ASSERT_EQ(0, noFunctions.size());

    // compare random dots to dots from deserialization
    {
        bool onlyMatchesFound = true;
        for(unsigned short i= 0; i < randomDots.size(); ++i)
        {
            auto & randomDot = randomDots[i];
            auto & dotFromPersistence = dotsFromPersistence[i];

            if(!(AreClose(randomDot->GetCoordinates().first, dotFromPersistence->GetCoordinates().first)
                 && AreClose(randomDot->GetCoordinates().second, dotFromPersistence->GetCoordinates().second)
                 && randomDot->IsGood() == dotFromPersistence->IsGood()
                 && randomDot->GetRadius() == dotFromPersistence->GetRadius()))
            {
                onlyMatchesFound = false;
                break;
            }
        }

        ASSERT_FALSE(onlyMatchesFound);
    }

    ASSERT_EQ(persistedDots.size(), dotsFromPersistence.size());

    // compare persisted dots to dots from deserialization
    {
        bool onlyMatchesFound = true;
        for(unsigned short i= 0; i < persistedDots.size(); ++i)
        {
            auto & persistedDot = persistedDots[i];
            auto & dotFromPersistence = dotsFromPersistence[i];

            if(!(AreClose(persistedDot->GetCoordinates().first, dotFromPersistence->GetCoordinates().first)
                 && AreClose(persistedDot->GetCoordinates().second, dotFromPersistence->GetCoordinates().second)
                 && persistedDot->IsGood() == dotFromPersistence->IsGood()
                 && persistedDot->GetRadius() == dotFromPersistence->GetRadius()))
            {
                onlyMatchesFound = false;
                break;
            }
        }

        ASSERT_TRUE(onlyMatchesFound);
    }

    ASSERT_EQ(5, functionsFromPersistence.size());
    EXPECT_STREQ(L"tan(x)", functionsFromPersistence[0].c_str());
    EXPECT_STREQ(L"x^3", functionsFromPersistence[1].c_str());
    EXPECT_STREQ(L"", functionsFromPersistence[2].c_str());
    EXPECT_STREQ(L"", functionsFromPersistence[3].c_str());
    EXPECT_STREQ(L"", functionsFromPersistence[4].c_str());
}

#endif // TST_DESERIALIZER_H
