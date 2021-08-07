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

#if defined(_SKIP_LONG_TEST)
#elif defined(_USE_LONG_TEST)
#else
#error "you need to make a choice between using or skipping long tests, -D_USE_LONG_TEST -D_SKIP_LONG_TEST"
#endif

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
#include "subsetgenerator.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, SimpleCasesShouldParseCorrectly)
{
    // Arrange
    Parser parser;
    std::wstring x = L"X";
    std::wstring y = L"Y";
    std::wstring number = L" +030.500 ";
    std::wstring invalidNumber = L"030.50.0";
    std::wstring bracedX = L"(X)";
    std::wstring doubleBracedX = L"((X))";

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
    std::wstring positive = L"+1.1";
    std::wstring negative = L"-2.2";

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
    std::wstring positiveX = L"+x";
    std::wstring negativeX = L"-x";

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
    std::wstring twoAdd = L"+2.0+3.0";

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
    std::wstring twoSubtract = L"-2.0-3.0";

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
    std::wstring twoAddBracketed = L"(2.0)+(3.0)";

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
    std::wstring threeAdd = L"2.0+3.0-x";

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
    std::wstring threeAddWithBrackets = L"2.0-(3.0+x)+1.0";

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
    std::wstring twoMultiply = L"2.0*3.0";

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
    std::wstring twoDivide = L"2.0/3.0";

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
    std::wstring twoMultiplyBracketed = L"(2.0)*(3.0)";

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
    std::wstring threeMultiply = L"2.0*3.0/x";

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
    std::wstring threeMultiplyWithBrackets = L"2.0/(3.0*x)*1.0";

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
    std::wstring threeTerms = L"2.0*x+1.0";

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
    std::wstring threeTerms = L"-2.0*x+1.0";

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
    std::wstring threeTermsBracketed = L"-2.1*(x+3.1)+1.1";

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
    std::wstring threeTermsBracketed = L"(x+3.1)*-2.1+1.1";

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
    std::wstring square = L"x^2.0";

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
    std::wstring invertedSquare = L"x^(-2.0)";

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
    std::wstring invertedSquare = L"x^-2.0";

    // Act
    auto exprPower = parser.Parse(invertedSquare);

    // Assert
    ASSERT_FALSE(exprPower);
}

TEST(BackendTest, PowerShouldParseCorrectly4)
{
    // Arrange
    Parser parser;
    std::wstring powerString = L"2.0^x";

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
    std::wstring powerString = L"-(2.0^x)";

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
    std::wstring powerString = L"-((2.0*x)^(x+1.0))";

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
    std::wstring powerString = L"3.0^x^2.0";

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
    std::wstring functionString = L"cos(x+sin(x))";

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
    std::wstring functionString = L"abs(sin(cos(tan(exp(ln(x))))))";

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
    std::wstring functionString1 = L"cis(x+sin(x))";
    std::wstring functionString2 = L"z(x+sin(x))";

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

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
    std::wstring string = optional.value_or(L"irrelevant");

    // Act
    auto expr = parser.Parse(string);

    // Assert
    ASSERT_TRUE(optional.has_value());
    EXPECT_EQ(*reference, *expr);
}

struct TestFunctionResult
{
    std::wstring testname;
    std::wstring text;
    bool expectedParseability;
    friend std::wostream& operator<<(std::wostream& os, const TestFunctionResult& obj)
    {
        return os
                << L"testname: " << obj.testname
                << L" text: " << obj.text
                << L" expectedParseability: " << (obj.expectedParseability ? L"true" : L"false");
    }
};

class ParseabilityTest : public testing::TestWithParam<TestFunctionResult>
{
};

INSTANTIATE_TEST_SUITE_P(BackendTest, ParseabilityTest,
    testing::Values(
    TestFunctionResult{L"Empty", L"", false},
    TestFunctionResult{L"X", L"X", true},
    TestFunctionResult{L"Y", L"Y", false},
    TestFunctionResult{L"Number", L" +030.500", true},
    TestFunctionResult{L"Invalid number", L"030.50.0", false},
    TestFunctionResult{L"Braced X", L"(X)", true},
    TestFunctionResult{L"Double braced X", L"((X))", true},
    TestFunctionResult{L"Complete sum", L"1+x+2", true},
    TestFunctionResult{L"Incomplete sum", L"1+x+", false},
    TestFunctionResult{L"Complete product", L"1/x", true},
    TestFunctionResult{L"Incomplete product", L"1/", false},
    TestFunctionResult{L"Complete power", L"1^x", true},
    TestFunctionResult{L"Incomplete power", L"1^", false},
    TestFunctionResult{L"Malformed", L"(x)x", false},
    TestFunctionResult{L"Function no argument", L"sin()", false},
    TestFunctionResult{L"Number with space", L"1 .0", true},
    TestFunctionResult{L"Double x 1", L"x x", false},
    TestFunctionResult{L"Double x 2", L"xx", false},
    TestFunctionResult{L"Braced double x", L"(x x)", false},
    TestFunctionResult{L"Open brace", L"", false},
    TestFunctionResult{L"Close brace", L"", false},
    TestFunctionResult{L"Just braces", L"()", false},
    TestFunctionResult{L"Missing operator", L"4x", false},
    TestFunctionResult{L"shadow01", L"+1.1", true},
    TestFunctionResult{L"shadow02", L"-2.2", true},
    TestFunctionResult{L"shadow03", L"+x", true},
    TestFunctionResult{L"shadow04", L"-x", true},
    TestFunctionResult{L"shadow05", L"+2.0+3.0", true},
    TestFunctionResult{L"shadow06", L"-2.0-3.0", true},
    TestFunctionResult{L"shadow07", L"(2.0)+(3.0)", true},
    TestFunctionResult{L"shadow08", L"2.0+3.0-x", true},
    TestFunctionResult{L"shadow09", L"2.0-(3.0+x)+1.0", true},
    TestFunctionResult{L"shadow10", L"2.0*3.0", true},
    TestFunctionResult{L"shadow11", L"2.0/3.0", true},
    TestFunctionResult{L"shadow12", L"(2.0)*(3.0)", true},
    TestFunctionResult{L"shadow13", L"2.0*3.0/x", true},
    TestFunctionResult{L"shadow14", L"2.0/(3.0*x)*1.0", true},
    TestFunctionResult{L"shadow15", L"2.0*x+1.0", true},
    TestFunctionResult{L"shadow16", L"-2.0*x+1.0", true},
    TestFunctionResult{L"shadow17", L"-2.1*(x+3.1)+1.1", true},
    TestFunctionResult{L"shadow18", L"(x+3.1)*-2.1+1.1", true},
    TestFunctionResult{L"shadow19", L"x^2.0", true},
    TestFunctionResult{L"shadow20", L"x^(-2.0)", true},
    TestFunctionResult{L"shadow21", L"x^-2.0", false},
    TestFunctionResult{L"shadow22", L"2.0^x", true},
    TestFunctionResult{L"shadow23", L"-(2.0^x)", true},
    TestFunctionResult{L"shadow24", L"-((2.0*x)^(x+1.0))", true},
    TestFunctionResult{L"shadow25", L"3.0^x^2.0", true},
    TestFunctionResult{L"shadow26", L"cos(x+sin(x))", true},
    TestFunctionResult{L"shadow27", L"abs(sin(cos(tan(exp(ln(x))))))", true},
    TestFunctionResult{L"shadow28", L"cis(x+sin(x))", false},
    TestFunctionResult{L"shadow29", L"z(x+sin(x))", false}
));

TEST_P(ParseabilityTest, CheckingForParseabilityShouldNotCrashAndYieldCorrectResult)
{
    // Arrange
    Parser parser;
    TestFunctionResult tfr = GetParam();

    // Act
    bool actualResult;

    if(tfr.text == L"")
    {
        actualResult = parser.IsParseable(tfr.text);
    }
    else
    {
#ifdef _SKIP_LONG_TEST
        if(tfr.text.length() < 9)
#else // _USE_LONG_TEST
        if(tfr.text.length() < 16)
#endif // _SKIP_LONG_TEST
        {
            // simulate typing - all subsets
            SubsetGenerator generator(tfr.text);
            while(generator.HasNext())
            {
                actualResult = parser.IsParseable(generator.GetNext());
            }
        }
        else
        {
            // simulate typing - linearly
            for(size_t i = 1; i <= tfr.text.length(); ++i)
            {
                auto substr = tfr.text.substr(0, i);
                actualResult = parser.IsParseable(substr);
            }
        }
    }

    // Assert
    EXPECT_EQ(tfr.expectedParseability, actualResult)
            << "testname: \"" << tfr.testname
            << "\" text: \"" << tfr.text
            << "\" expectedParseability: " << (tfr.expectedParseability ? "true" : "false");
}

#endif // TST_PARSER_H
