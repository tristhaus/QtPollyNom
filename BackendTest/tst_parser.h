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
#include "../Backend/power.h"
#include "../Backend/parser.h"
#include "../Backend/functions.h"
#include "testexpressionbuilder.h"

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

    EXPECT_EQ(referenceX, *exprX);
    EXPECT_FALSE(exprInvalid);
    EXPECT_EQ(referenceConstant, *exprNumber);
    EXPECT_FALSE(exprInvalidNumber);
    EXPECT_EQ(referenceX, *exprBracedX);
    EXPECT_EQ(referenceX, *exprDoubleBracedX);
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

    EXPECT_EQ(referencePositive, *exprPositive);
    EXPECT_EQ(referenceNegative, *exprNegative);
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
    EXPECT_EQ(*referencePositiveX, *exprPositive);
    ASSERT_TRUE(exprNegative);
    EXPECT_EQ(referenceNegativeX, *exprNegative);
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
    EXPECT_EQ(referenceSum, *exprSum);
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
    EXPECT_EQ(referenceSum, *exprSum);
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
    EXPECT_EQ(referenceSum, *exprSum);
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
    EXPECT_EQ(referenceSum, *exprSum);
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
    EXPECT_EQ(referenceSum, *exprSum);
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
    EXPECT_EQ(referenceProduct, *exprProduct);
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
    EXPECT_EQ(referenceProduct, *exprProduct);
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
    EXPECT_EQ(referenceProduct, *exprProduct);
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
    EXPECT_EQ(referenceProduct, *exprProduct);
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
    EXPECT_EQ(referenceProduct, *exprProduct);
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
    EXPECT_EQ(referenceSum, *exprProduct);
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
    EXPECT_EQ(referenceSum, *exprProduct);
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
    EXPECT_EQ(referenceSum, *exprProduct);
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
    EXPECT_EQ(referenceSum, *exprProduct);
}

TEST(BackendTest, PowerShouldParseCorrectly1)
{
    // Arrange
    Parser parser;
    std::string square = "x^2.0";

    // Act
    auto exprPower = parser.Parse(square);

    // Assert
    auto x = std::make_shared<BaseX>();
    auto constant = std::make_shared<Constant>(2.0);

    Power referencePower(x, constant);

    ASSERT_TRUE(exprPower);
    EXPECT_EQ(referencePower, *exprPower);
}

TEST(BackendTest, PowerShouldParseCorrectly2)
{
    // Arrange
    Parser parser;
    std::string invertedSquare = "x^(-2.0)";

    // Act
    auto exprPower = parser.Parse(invertedSquare);

    // Assert
    auto x = std::make_shared<BaseX>();
    auto constant = std::make_shared<Constant>(-2.0);

    Power referencePower(x, constant);

    ASSERT_TRUE(exprPower);
    EXPECT_EQ(referencePower, *exprPower);
}

TEST(BackendTest, PowerShouldParseCorrectly3)
{
    // Arrange
    Parser parser;
    std::string invertedSquare = "x^-2.0";

    // Act
    auto exprPower = parser.Parse(invertedSquare);

    // Assert
    ASSERT_FALSE(exprPower);
}

TEST(BackendTest, PowerShouldParseCorrectly4)
{
    // Arrange
    Parser parser;
    std::string powerString = "2.0^x";

    // Act
    auto exprPower = parser.Parse(powerString);

    // Assert
    auto constant = std::make_shared<Constant>(2.0);
    auto x = std::make_shared<BaseX>();

    Power referencePower(constant, x);

    ASSERT_TRUE(exprPower);
    EXPECT_EQ(referencePower, *exprPower);
}

TEST(BackendTest, PowerShouldParseCorrectly5)
{
    // Arrange
    Parser parser;
    std::string powerString = "-(2.0^x)";

    // Act
    auto exprPower = parser.Parse(powerString);

    // Assert
    auto constant = std::make_shared<Constant>(2.0);
    auto x = std::make_shared<BaseX>();

    auto powerTerm = std::make_shared<Power>(constant, x);
    Sum referencePower({Sum::Summand(Sum::Sign::Minus, powerTerm)});

    ASSERT_TRUE(exprPower);
    EXPECT_EQ(referencePower, *exprPower);
}

TEST(BackendTest, LessSimplePowerShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::string powerString = "-((2.0*x)^(x+1.0))";

    // Act
    auto exprProduct = parser.Parse(powerString);

    // Assert
    auto constant1 = std::make_shared<Constant>(1.0);
    auto constant2 = std::make_shared<Constant>(2.0);
    auto x = std::make_shared<BaseX>();

    auto bracketProduct = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant2), Product::Factor(Product::Exponent::Positive, x)});

    auto bracketSum = std::make_shared<Sum>(std::vector<Sum::Summand>{Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Plus, constant1)});

    auto powerTerm = std::make_shared<Power>(bracketProduct, bracketSum);

    Sum referencePower({Sum::Summand(Sum::Sign::Minus, powerTerm)});

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(referencePower, *exprProduct);
}

TEST(BackendTest, PowerTowerShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::string powerString = "3.0^x^2.0";

    // Act
    auto exprProduct = parser.Parse(powerString);

    // Assert
    auto constant1 = std::make_shared<Constant>(3.0);
    auto constant2 = std::make_shared<Constant>(2.0);
    auto x = std::make_shared<BaseX>();

    auto upperPower = std::make_shared<Power>(x, constant2);

    Power referencePower(constant1, upperPower);

    ASSERT_TRUE(exprProduct);
    EXPECT_EQ(referencePower, *exprProduct);
}

TEST(BackendTest, FunctionsShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::string functionString = "cos(x+sin(x))";

    // Act
    auto exprFunction = parser.Parse(functionString);

    // Assert
    auto x = std::make_shared<BaseX>();
    auto sine = std::make_shared<Sine>(x);
    auto sum = std::make_shared<Sum>(std::vector<Sum::Summand>{Sum::Summand(Sum::Sign::Plus, x), Sum::Summand(Sum::Sign::Plus, sine)});
    auto referenceExpression = std::make_shared<Cosine>(sum);

    ASSERT_TRUE(exprFunction);
    EXPECT_EQ(*referenceExpression, *exprFunction);
}

TEST(BackendTest, FunctionsTowerShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::string functionString = "abs(sin(cos(tan(exp(ln(x))))))";

    // Act
    auto exprFunction = parser.Parse(functionString);

    // Assert
    auto x = std::make_shared<BaseX>();
    auto ln = std::make_shared<NaturalLogarithm>(x);
    auto exp = std::make_shared<NaturalExponential>(ln);
    auto tan = std::make_shared<Tangent>(exp);
    auto cos = std::make_shared<Cosine>(tan);
    auto sin = std::make_shared<Sine>(cos);
    auto referenceExpression = std::make_shared<AbsoluteValue>(sin);

    ASSERT_TRUE(exprFunction);
    EXPECT_EQ(*referenceExpression, *exprFunction);
}

TEST(BackendTest, InvalidFunctionsShouldFailToParse)
{
    // Arrange
    Parser parser;
    std::string functionString1 = "cis(x+sin(x))";
    std::string functionString2 = "z(x+sin(x))";

    // Act
    auto exprFunction1 = parser.Parse(functionString1);
    auto exprFunction2 = parser.Parse(functionString2);

    // Assert
    EXPECT_FALSE(exprFunction1);
    EXPECT_FALSE(exprFunction2);
}

TEST(BackendTest, ComplexExpression01ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build01();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression02ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build02();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression03ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build03();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression04ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build04();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression05ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build05();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression06ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build06();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression07ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build07();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

TEST(BackendTest, ComplexExpression08ShouldRoundtripCorrectly)
{
    // Arrange
    Parser parser;
    auto reference = TestExpressionBuilder::Build08();
    auto optional = reference->Print();
    std::string string = optional.value_or("irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

struct TestFunctionResult
{
    std::string testname;
    std::string text;
    bool expectedParseability;
    friend std::ostream& operator<<(std::ostream& os, const TestFunctionResult& obj)
    {
        return os
                << "testname: " << obj.testname
                << " text: " << obj.text
                << " expectedParseability: " << (obj.expectedParseability ? "true" : "false");
    }
};

class ParseabilityTest : public testing::TestWithParam<TestFunctionResult>
{
};

INSTANTIATE_TEST_SUITE_P(BackendTest, ParseabilityTest,
    testing::Values(
    TestFunctionResult{std::string("Empty"), std::string(""), false},
    TestFunctionResult{std::string("X"), std::string("X"), true},
    TestFunctionResult{std::string("Y"), std::string("Y"), false},
    TestFunctionResult{std::string("Number"), std::string(" +030.500 "), true},
    TestFunctionResult{std::string("Invalid number"), std::string("030.50.0"), false},
    TestFunctionResult{std::string("Braced X"), std::string("(X)"), true},
    TestFunctionResult{std::string("Double braced X"), std::string("((X))"), true},
    TestFunctionResult{std::string("Complete sum"), std::string("1+x+2"), true},
    TestFunctionResult{std::string("Incomplete sum"), std::string("1+x+"), false},
    TestFunctionResult{std::string("Complete product"), std::string("1/x"), true},
    TestFunctionResult{std::string("Incomplete product"), std::string("1/"), false},
    TestFunctionResult{std::string("Complete power"), std::string("1^x"), true},
    TestFunctionResult{std::string("Incomplete power"), std::string("1^"), false},
    TestFunctionResult{std::string("Malformed"), std::string("(x)x"), false},
    TestFunctionResult{std::string("Function no argument"), std::string("sin()"), false},
    TestFunctionResult{std::string("Number with space"), std::string("1 .0"), true},
    TestFunctionResult{std::string("Double x 1"), std::string("x x"), false},
    TestFunctionResult{std::string("Double x 2"), std::string("xx"), false},
    TestFunctionResult{std::string("Braced double x"), std::string("(x x)"), false},
    TestFunctionResult{std::string("Open brace"), std::string("("), false},
    TestFunctionResult{std::string("Close brace"), std::string(")"), false},
    TestFunctionResult{std::string("Just braces"), std::string("()"), false},
    TestFunctionResult{std::string("Missing operator"), std::string("4x"), false}
));

TEST_P(ParseabilityTest, CheckingForParseabilityShouldNotCrashAndYieldCorrectResult)
{
    // Arrange
    Parser parser;
    TestFunctionResult tfr = GetParam();

    // Act
    bool actualResult;

    if(tfr.text == "")
    {
        actualResult = parser.IsParseable(tfr.text);
    }
    else
    {
        // simulate typing
        for(size_t i = 1; i <= tfr.text.length(); ++i)
        {
            auto substr = tfr.text.substr(0, i);
            actualResult = parser.IsParseable(substr);
        }
    }

    // Assert
    EXPECT_EQ(tfr.expectedParseability, actualResult)
            << "testname: \"" << tfr.testname
            << "\" text: \"" << tfr.text
            << "\" expectedParseability: " << (tfr.expectedParseability ? "true" : "false");
}

#endif // TST_PARSER_H
