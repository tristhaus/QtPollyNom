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

#ifndef TST_PARSER_H
#define TST_PARSER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"
#include "../Backend/parser.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, SimpleCasesShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::string x = "X";
    std::string y = "Y";
    std::string number = " +030.500 ";
    std::string invalidNumber = "030.50.0";
    std::string bracedX = "(X)";
    std::string doubleBracedX = "((X))";

    // Act
    auto exprX = parser.Parse(x);
    auto exprInvalid = parser.Parse(y);
    auto exprNumber = parser.Parse(number);
    auto exprInvalidNumber = parser.Parse(invalidNumber);
    auto exprBracedX = parser.Parse(bracedX);
    auto exprDoubleBracedX = parser.Parse(doubleBracedX);

    // Assert
    BaseX referenceX;
    Constant referenceConstant(30.5);

    EXPECT_EQ(*exprX, referenceX);
    EXPECT_FALSE(exprInvalid);
    EXPECT_EQ(*exprNumber, referenceConstant);
    EXPECT_FALSE(exprInvalidNumber);
    EXPECT_EQ(*exprBracedX, referenceX);
    EXPECT_EQ(*exprDoubleBracedX, referenceX);
}

TEST(BackendTest, SingleSummandShouldParseToConstant)
{
    // Arrange
    Parser parser;
    std::string positive = "+1.1";
    std::string negative = "-2.2";

    // Act
    auto exprPositive = parser.Parse(positive);
    auto exprNegative = parser.Parse(negative);

    // Assert
    Constant referencePositive(+1.1);
    Constant referenceNegative(-2.2);

    EXPECT_EQ(*exprPositive, referencePositive);
    EXPECT_EQ(*exprNegative, referenceNegative);
}

TEST(BackendTest, SingleXShouldParseToSum)
{
    // Arrange
    Parser parser;
    std::string positiveX = "+x";
    std::string negativeX = "-x";

    // Act
    auto exprPositive = parser.Parse(positiveX);
    auto exprNegative = parser.Parse(negativeX);

    // Assert
    auto referencePositiveX = std::make_shared<BaseX>();
    Sum referenceNegativeX = Sum({Sum::Summand(Sum::Sign::Minus, referencePositiveX)});

    ASSERT_TRUE(exprPositive);
    EXPECT_EQ(*exprPositive, *referencePositiveX);
    ASSERT_TRUE(exprNegative);
    EXPECT_EQ(*exprNegative, referenceNegativeX);
}

TEST(BackendTest, SimpleSumShouldParseCorrectly1)
{
    // Arrange
    Parser parser;
    std::string twoAdd = "+2.0+3.0";

    // Act
    auto exprSum = parser.Parse(twoAdd);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, constant1), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprSum);
    EXPECT_EQ(*exprSum, referenceSum);
}

TEST(BackendTest, SimpleSumShouldParseCorrectly2)
{
    // Arrange
    Parser parser;
    std::string twoSubtract = "-2.0-3.0";

    // Act
    auto exprSum = parser.Parse(twoSubtract);

    // Assert
    auto constant1 = std::make_shared<Constant>(-2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, constant1), Sum::Summand(Sum::Sign::Minus, constant2)});

    ASSERT_TRUE(exprSum);
    EXPECT_EQ(*exprSum, referenceSum);
}

TEST(BackendTest, SimpleSumShouldParseCorrectly3)
{
    // Arrange
    Parser parser;
    std::string twoAddBracketed = "(2.0)+(3.0)";

    // Act
    auto exprSum = parser.Parse(twoAddBracketed);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, constant1), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprSum);
    EXPECT_EQ(*exprSum, referenceSum);
}

TEST(BackendTest, SimpleSumShouldParseCorrectly4)
{
    // Arrange
    Parser parser;
    std::string threeAdd = "2.0+3.0-x";

    // Act
    auto exprSum = parser.Parse(threeAdd);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();
    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, constant1), Sum::Summand(Sum::Sign::Plus, constant2), Sum::Summand(Sum::Sign::Minus, x)});

    ASSERT_TRUE(exprSum);
    EXPECT_EQ(*exprSum, referenceSum);
}

TEST(BackendTest, SimpleSumShouldParseCorrectly5)
{
    // Arrange
    Parser parser;
    std::string threeAddWithBrackets = "2.0-(3.0+x)+1.0";

    // Act
    auto exprSum = parser.Parse(threeAddWithBrackets);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();
    auto constant3 = std::make_shared<Constant>(1.0);

    auto innerBracket = std::make_shared<Sum>(std::vector<Sum::Summand>{Sum::Summand(Sum::Sign::Plus, constant2), Sum::Summand(Sum::Sign::Plus, x)});

    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, constant1), Sum::Summand(Sum::Sign::Minus, innerBracket), Sum::Summand(Sum::Sign::Plus, constant3)});

    ASSERT_TRUE(exprSum);
    EXPECT_EQ(*exprSum, referenceSum);
}

TEST(BackendTest, SimpleProductShouldParseCorrectly1)
{
    // Arrange
    Parser parser;
    std::string twoMultiply = "2.0*3.0";

    // Act
    auto exprProduct = parser.Parse(twoMultiply);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);

    Product referenceProduct = Product({Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceProduct);
}

TEST(BackendTest, SimpleProductShouldParseCorrectly2)
{
    // Arrange
    Parser parser;
    std::string twoDivide = "2.0/3.0";

    // Act
    auto exprProduct = parser.Parse(twoDivide);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);

    Product referenceProduct = Product({Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Negative, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceProduct);
}

TEST(BackendTest, SimpleProductShouldParseCorrectly3)
{
    // Arrange
    Parser parser;
    std::string twoMultiplyBracketed = "(2.0)*(3.0)";

    // Act
    auto exprProduct = parser.Parse(twoMultiplyBracketed);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);

    Product referenceProduct = Product({Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceProduct);
}

TEST(BackendTest, SimpleProductShouldParseCorrectly4)
{
    // Arrange
    Parser parser;
    std::string threeMultiply = "2.0*3.0/x";

    // Act
    auto exprProduct = parser.Parse(threeMultiply);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();
    Product referenceProduct = Product({Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, constant2), Product::Factor(Product::Exponent::Negative, x)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceProduct);
}

TEST(BackendTest, SimpleProductShouldParseCorrectly5)
{
    // Arrange
    Parser parser;
    std::string threeMultiplyWithBrackets = "2.0/(3.0*x)*1.0";

    // Act
    auto exprProduct = parser.Parse(threeMultiplyWithBrackets);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();
    auto constant3 = std::make_shared<Constant>(1.0);

    auto innerBracket = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant2), Product::Factor(Product::Exponent::Positive, x)});

    Product referenceProduct = Product({Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Negative, innerBracket), Product::Factor(Product::Exponent::Positive, constant3)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceProduct);
}

TEST(BackendTest, ProductSumMixShouldParseCorrectly1)
{
    // Arrange
    Parser parser;
    std::string threeTerms = "2.0*x+1.0";

    // Act
    auto exprProduct = parser.Parse(threeTerms);

    // Assert
    auto constant1 = std::make_shared<Constant>(2.0);
    auto constant2 = std::make_shared<Constant>(1.0);
    auto x = std::make_shared<BaseX>();

    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, x)});

    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, product), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceSum);
}

TEST(BackendTest, ProductSumMixShouldParseCorrectly2)
{
    // Arrange
    Parser parser;
    std::string threeTerms = "-2.0*x+1.0";

    // Act
    auto exprProduct = parser.Parse(threeTerms);

    // Assert
    auto constant1 = std::make_shared<Constant>(-2.0);
    auto constant2 = std::make_shared<Constant>(1.0);
    auto x = std::make_shared<BaseX>();

    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, x)});

    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, product), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceSum);
}

TEST(BackendTest, ProductSumMixShouldParseCorrectly3)
{
    // Arrange
    Parser parser;
    std::string threeTermsBracketed = "-2.1*(x+3.1)+1.1";

    // Act
    auto exprProduct = parser.Parse(threeTermsBracketed);

    // Assert
    auto constant1 = std::make_shared<Constant>(-2.1);
    auto constant2 = std::make_shared<Constant>(1.1);
    auto constant3 = std::make_shared<Constant>(3.1);
    auto x = std::make_shared<BaseX>();

    auto bracketSum = std::make_shared<Sum>(std::vector<Sum::Summand>{Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Plus, constant3)});

    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant1), Product::Factor(Product::Exponent::Positive, bracketSum)});

    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, product), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceSum);
}

TEST(BackendTest, ProductSumMixShouldParseCorrectly4)
{
    // Arrange
    Parser parser;
    std::string threeTermsBracketed = "(x+3.1)*-2.1+1.1";

    // Act
    auto exprProduct = parser.Parse(threeTermsBracketed);

    // Assert
    auto constant1 = std::make_shared<Constant>(-2.1);
    auto constant2 = std::make_shared<Constant>(1.1);
    auto constant3 = std::make_shared<Constant>(3.1);
    auto x = std::make_shared<BaseX>();

    auto bracketSum = std::make_shared<Sum>(std::vector<Sum::Summand>{Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Plus, constant3)});

    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, bracketSum), Product::Factor(Product::Exponent::Positive, constant1)});

    Sum referenceSum = Sum({Sum::Summand(Sum::Sign::Plus, product), Sum::Summand(Sum::Sign::Plus, constant2)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(*exprProduct, referenceSum);
}

#endif // TST_PARSER_H
