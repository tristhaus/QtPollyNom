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

#include <sstream>

using namespace testing;
using namespace Backend;

TEST(BackendTest, SerializationOfGame1WithoutFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::stringstream ss;
    DeSerializer ds;
    Game game(std::make_shared<FixedDotGenerator>());

    // Act
    ds.Serialize(game, ss);

    // Assert
    auto result = ss.str();

    ASSERT_TRUE(result.length() > 0);
    std::regex dataVersionRegex(R"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dataVersionRegex));
    std::regex creationDateRegex(R"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, creationDateRegex));
    std::regex dotsRegex(R"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dotsRegex));
    std::regex functionsRegex(R"foo("functions":\[\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, functionsRegex));
}

TEST(BackendTest, SerializationOfGame1WithFunctionsShouldWorkCorrectly)
{
    // Arrange
    std::stringstream ss;
    DeSerializer ds;
    Game game(std::make_shared<FixedDotGenerator>());
    game.Update(std::vector<std::string> {"sin(x)", "x^2", "abc"});

    // Act
    ds.Serialize(game, ss);

    // Assert
    auto result = ss.str();

    ASSERT_TRUE(result.length() > 0);
    std::regex dataVersionRegex(R"foo("dataVersion":"[0-9]+")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dataVersionRegex));
    std::regex creationDateRegex(R"foo("creationDate":")foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, creationDateRegex));
    std::regex dotsRegex(R"foo("dots":\[\{"x":1\.0,"y":1\.0,"radius":0\.25,"kind":"good"\},\{"x":-8\.0,"y":-0\.25,"radius":0\.25,"kind":"good"\},\{"x":-4\.0,"y":0\.35,"radius":0\.25,"kind":"good"\},\{"x":5\.0,"y":-5\.0,"radius":0\.25,"kind":"good"\},\{"x":2.5,"y":5.0,"radius":0.25,"kind":"bad"\}\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, dotsRegex));
    std::regex functionsRegex(R"foo("functions":\["sin\(x\)","x\^2","abc"\])foo", std::regex_constants::ECMAScript);
    EXPECT_TRUE(std::regex_search(result, functionsRegex));
}

#endif // TST_DESERIALIZER_H
