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

#ifndef TST_EQUALITY_H
#define TST_EQUALITY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"
#include "../Backend/power.h"
#include "../Backend/functions.h"

using namespace testing;
using namespace Backend;

#include <memory>

TEST(BackendTest, EqualityShallWorkCorrectlyUsingRawPointers)
{
    // Arrange
    Constant constant1(4.8);
    Constant constant2(4.8);
    BaseX baseX1;
    BaseX baseX2;

    const Expression * const pC1 = &constant1;
    const Expression * const pC2 = &constant2;
    const Expression * const pB1 = &baseX1;
    const Expression * const pB2 = &baseX2;

    // Act, Assert
    ASSERT_TRUE(*pC1 == *pC2);
    ASSERT_TRUE(*pB1 == *pB2);
    ASSERT_FALSE(*pB1 == *pC2);
    ASSERT_FALSE(*pC1 != *pC2);
    ASSERT_FALSE(*pC1 != *pC1);

    ASSERT_EQ(*pC1, *pC2);
    ASSERT_EQ(*pB1, *pB2);
    ASSERT_NE(*pC1, *pB1);
}

TEST(BackendTest, EqualityShallWorkCorrectlyUsingSmartPointers)
{
    // Arrange
    std::shared_ptr<Constant> constant1 = std::make_shared<Constant>(4.8);
    std::shared_ptr<Constant> constant2 = std::make_shared<Constant>(4.8);
    std::shared_ptr<BaseX> baseX1 = std::make_shared<BaseX>();
    std::shared_ptr<BaseX> baseX2 = std::make_shared<BaseX>();

    std::shared_ptr<Expression> pC1(constant1);
    std::shared_ptr<Expression> pC2(constant2);
    std::shared_ptr<Expression> pB1(baseX1);
    std::shared_ptr<Expression> pB2(baseX2);

    // Act, Assert
    ASSERT_TRUE(*pC1 == *pC2);
    ASSERT_TRUE(*pB1 == *pB2);
    ASSERT_FALSE(*pB1 == *pC2);
    ASSERT_FALSE(*pC1 != *pC2);
    ASSERT_FALSE(*pC1 != *pC1);

    ASSERT_EQ(*pC1, *pC2);
    ASSERT_EQ(*pB1, *pB2);
    ASSERT_NE(*pC1, *pB1);
}

TEST(BackendTest, EqualityShallWorkCorrectlyUsingReferences)
{
    // Arrange
    Constant constant1(4.8);
    Constant constant2(4.8);
    BaseX baseX1;
    BaseX baseX2;

    const Expression& pC1 = constant1;
    const Expression& pC2 = constant2;
    const Expression& pB1 = baseX1;
    const Expression& pB2 = baseX2;

    // Act, Assert
    ASSERT_TRUE(pC1 == pC2);
    ASSERT_TRUE(pB1 == pB2);
    ASSERT_FALSE(pB1 == pC2);
    ASSERT_FALSE(pC1 != pC2);
    ASSERT_FALSE(pC1 != pC1);

    ASSERT_EQ(pC1, pC2);
    ASSERT_EQ(pB1, pB2);
    ASSERT_NE(pC1, pB1);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForSummands)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x3 = std::make_shared<BaseX>();

    auto x1Summand = Sum::Summand(Sum::Sign::Plus, x1);
    auto x2Summand = Sum::Summand(Sum::Sign::Plus, x2);
    auto x3Summand = Sum::Summand(Sum::Sign::Minus, x3);

    std::shared_ptr<Expression> c1 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Expression> c2 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Expression> c3 = std::make_shared<Constant>(4.0);

    auto c1Summand = Sum::Summand(Sum::Sign::Minus, c1);
    auto c2Summand = Sum::Summand(Sum::Sign::Minus, c2);
    auto c3Summand = Sum::Summand(Sum::Sign::Minus, c3);

    // Act, Assert
    ASSERT_TRUE(x1Summand == x1Summand);
    ASSERT_TRUE(x1Summand == x2Summand);
    ASSERT_FALSE(x1Summand == x3Summand);

    ASSERT_FALSE(x1Summand != x1Summand);
    ASSERT_FALSE(x1Summand != x2Summand);
    ASSERT_TRUE(x1Summand != x3Summand);

    ASSERT_TRUE(c1Summand == c1Summand);
    ASSERT_TRUE(c1Summand == c2Summand);
    ASSERT_FALSE(c1Summand == c3Summand);

    ASSERT_FALSE(c1Summand != c1Summand);
    ASSERT_FALSE(c1Summand != c2Summand);
    ASSERT_TRUE(c1Summand != c3Summand);

    ASSERT_EQ(x1Summand, x1Summand);
    ASSERT_EQ(x1Summand, x2Summand);
    ASSERT_NE(x1Summand, x3Summand);

    ASSERT_EQ(c1Summand, c1Summand);
    ASSERT_EQ(c1Summand, c2Summand);
    ASSERT_NE(c1Summand, c3Summand);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForSums)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c1 = std::make_shared<Constant>(3.0);

    auto x1Summand = Sum::Summand(Sum::Sign::Plus, x1);  // x
    auto x2Summand = Sum::Summand(Sum::Sign::Minus, x2); // - x
    auto c1Summand = Sum::Summand(Sum::Sign::Plus, c1);  // 3.0

    Sum s1 = Sum({x1Summand, x1Summand});            // x + x
    Sum s2 = Sum({x1Summand, x1Summand});            // x + x
    Sum s3 = Sum({x1Summand, x2Summand});            // x - x
    Sum s4 = Sum({x1Summand, x1Summand, x1Summand}); // x + x + x
    Sum s5 = Sum({x2Summand, x1Summand});            // - x + x
    Sum s6 = Sum({c1Summand, x1Summand, x2Summand}); // 3.0 + x - x
    Sum s7 = Sum({x2Summand, c1Summand, x1Summand}); // - x + 3.0 + x

    // Act, Assert
    ASSERT_TRUE(s1 == s2);
    ASSERT_FALSE(s1 == s3);
    ASSERT_FALSE(s1 == s4);
    ASSERT_TRUE(s3 == s5);
    ASSERT_TRUE(s6 == s7);

    ASSERT_FALSE(s1 != s2);
    ASSERT_TRUE(s1 != s3);
    ASSERT_TRUE(s1 != s4);
    ASSERT_FALSE(s3 != s5);
    ASSERT_FALSE(s6 != s7);

    ASSERT_EQ(s1, s2);
    ASSERT_NE(s1, s3);
    ASSERT_NE(s1, s4);
    ASSERT_EQ(s3, s5);
    ASSERT_EQ(s6, s7);
}

// while we do not consider ( a + b ) + c == a + ( b + c ) to be true (associativity)
// we should have commutativity even when recursive comparison is needed, so
// ( a + b ) + c == c + ( b + a ) should be true
TEST(BackendTest, EqualityShallWorkCorrectlyForRecursiveSums)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c = std::make_shared<Constant>(3.0);

    auto x1Summand = Sum::Summand(Sum::Sign::Plus, x1);  // x
    auto x2Summand = Sum::Summand(Sum::Sign::Minus, x2); // -x
    auto cSummand = Sum::Summand(Sum::Sign::Minus, c);   // -3.0

    std::shared_ptr<Expression> s1 = std::make_shared<Sum>(std::vector<Sum::Summand>({x1Summand, x1Summand})); // x + x
    std::shared_ptr<Expression> s2 = std::make_shared<Sum>(std::vector<Sum::Summand>({x1Summand, x2Summand})); // x - x

    auto s1Summand = Sum::Summand(Sum::Sign::Plus, s1);  // x + x
    auto s2Summand = Sum::Summand(Sum::Sign::Minus, s2); // -(x - x)

    std::shared_ptr<Expression> s3 = std::make_shared<Sum>(std::vector<Sum::Summand>({cSummand, s1Summand, s2Summand})); // - 3.0 + ( x + x ) - ( x - x )
    std::shared_ptr<Expression> s4 = std::make_shared<Sum>(std::vector<Sum::Summand>({s2Summand, s1Summand, cSummand})); // - ( x - x ) + ( x + x ) - 3.0

    ASSERT_TRUE(*s3 == *s4);
    ASSERT_FALSE(*s3 != *s4);

    ASSERT_EQ(*s3, *s4);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForFactors)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x3 = std::make_shared<BaseX>();

    auto x1Factor = Product::Factor(Product::Exponent::Positive, x1);
    auto x2Factor = Product::Factor(Product::Exponent::Positive, x2);
    auto x3Factor = Product::Factor(Product::Exponent::Negative, x3);

    std::shared_ptr<Expression> c1 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Expression> c2 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Expression> c3 = std::make_shared<Constant>(4.0);

    auto c1Factor = Product::Factor(Product::Exponent::Positive, c1);
    auto c2Factor = Product::Factor(Product::Exponent::Positive, c2);
    auto c3Factor = Product::Factor(Product::Exponent::Negative, c3);

    // Act, Assert
    ASSERT_TRUE(x1Factor == x1Factor);
    ASSERT_TRUE(x1Factor == x2Factor);
    ASSERT_FALSE(x1Factor == x3Factor);

    ASSERT_FALSE(x1Factor != x1Factor);
    ASSERT_FALSE(x1Factor != x2Factor);
    ASSERT_TRUE(x1Factor != x3Factor);

    ASSERT_TRUE(c1Factor == c1Factor);
    ASSERT_TRUE(c1Factor == c2Factor);
    ASSERT_FALSE(c1Factor == c3Factor);

    ASSERT_FALSE(c1Factor != c1Factor);
    ASSERT_FALSE(c1Factor != c2Factor);
    ASSERT_TRUE(c1Factor != c3Factor);

    ASSERT_EQ(x1Factor, x1Factor);
    ASSERT_EQ(x1Factor, x2Factor);
    ASSERT_NE(x1Factor, x3Factor);

    ASSERT_EQ(c1Factor, c1Factor);
    ASSERT_EQ(c1Factor, c2Factor);
    ASSERT_NE(c1Factor, c3Factor);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForProducts)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c1 = std::make_shared<Constant>(3.0);

    auto x1Factor = Product::Factor(Product::Exponent::Positive, x1);
    auto x2Factor = Product::Factor(Product::Exponent::Negative, x2);
    auto c1Factor = Product::Factor(Product::Exponent::Positive, c1);

    Product p1 = Product({x1Factor, x1Factor});           // x * x
    Product p2 = Product({x1Factor, x1Factor});           // x * x
    Product p3 = Product({x1Factor, x2Factor});           // x / x
    Product p4 = Product({x1Factor, x1Factor, x1Factor}); // x * x * x
    Product p5 = Product({x2Factor, x1Factor});           // 1 / x * x
    Product p6 = Product({c1Factor, x1Factor, x2Factor}); // 3.0 * x / x
    Product p7 = Product({x2Factor, c1Factor, x1Factor}); // 1 / x * 3.0 * x

    // Act, Assert
    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 == p3);
    ASSERT_FALSE(p1 == p4);
    ASSERT_TRUE(p3 == p5);
    ASSERT_TRUE(p6 == p7);

    ASSERT_FALSE(p1 != p2);
    ASSERT_TRUE(p1 != p3);
    ASSERT_TRUE(p1 != p4);
    ASSERT_FALSE(p3 != p5);
    ASSERT_FALSE(p6 != p7);

    ASSERT_EQ(p1, p2);
    ASSERT_NE(p1, p3);
    ASSERT_NE(p1, p4);
    ASSERT_EQ(p3, p5);
    ASSERT_EQ(p6, p7);
}

// while we do not consider ( a * b ) * c == a * ( b * c ) to be true (associativity)
// we should have commutativity even when recursive comparison is needed, so
// ( a * b ) * c == c * ( b * a ) should be true
TEST(BackendTest, EqualityShallWorkCorrectlyForRecursiveProducts)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c = std::make_shared<Constant>(3.0);

    auto x1Factor = Product::Factor(Product::Exponent::Positive, x1); // x
    auto x2Factor = Product::Factor(Product::Exponent::Negative, x2); // 1 / x
    auto cFactor = Product::Factor(Product::Exponent::Negative, c);   // 1 / 3.0

    std::shared_ptr<Expression> p1 = std::make_shared<Product>(std::vector<Product::Factor>({x1Factor, x1Factor})); // x * x
    std::shared_ptr<Expression> p2 = std::make_shared<Product>(std::vector<Product::Factor>({x1Factor, x2Factor})); // x / x

    auto s1Factor = Product::Factor(Product::Exponent::Positive, p1); // 1 * ( x * x )
    auto s2Factor = Product::Factor(Product::Exponent::Negative, p2); // 1 / ( x * x )

    std::shared_ptr<Expression> p3 = std::make_shared<Product>(std::vector<Product::Factor>({cFactor, s1Factor, s2Factor})); // 1 / 3.0 * ( x * x ) / ( x / x )
    std::shared_ptr<Expression> p4 = std::make_shared<Product>(std::vector<Product::Factor>({s2Factor, s1Factor, cFactor})); // 1 / ( x / x ) * ( x * x ) / 3.0

    ASSERT_TRUE(*p3 == *p4);
    ASSERT_FALSE(*p3 != *p4);

    ASSERT_EQ(*p3, *p4);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForPowers)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> x2 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c1 = std::make_shared<Constant>(3.0);

    Power q1 = Power(x1, c1); // x ^ 3.0
    Power q2 = Power(x2, c1); // x ^ 3.0
    Power q3 = Power(c1, x2); // 3.0 ^ x
    Power q4 = Power(x1, x2); // x ^ x

    // Act, Assert
    ASSERT_TRUE(q1 == q2);
    ASSERT_FALSE(q1 == q3);
    ASSERT_FALSE(q1 == q4);

    ASSERT_FALSE(q1 != q2);
    ASSERT_TRUE(q1 != q3);
    ASSERT_TRUE(q1 != q4);

    ASSERT_EQ(q1, q2);
    ASSERT_NE(q1, q3);
    ASSERT_NE(q1, q4);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForRecursivePowers)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c = std::make_shared<Constant>(3.0);

    std::shared_ptr<Expression> q1 = std::make_shared<Power>(x1, x1); // x ^ x
    std::shared_ptr<Expression> q2 = std::make_shared<Power>(x1, c);  // x ^ 3.0

    std::shared_ptr<Expression> q3 = std::make_shared<Power>(x1, q1); // x ^ ( x ^ x )
    std::shared_ptr<Expression> q4 = std::make_shared<Power>(x1, q1); // x ^ ( x ^ x )
    std::shared_ptr<Expression> q5 = std::make_shared<Power>(x1, q2); // x ^ ( x ^ 3.0 )

    // Act, Assert
    ASSERT_TRUE(*q3 == *q4);
    ASSERT_FALSE(*q3 == *q5);

    ASSERT_FALSE(*q3 != *q4);
    ASSERT_TRUE(*q3 != *q5);

    ASSERT_EQ(*q3, *q4);
    ASSERT_NE(*q3, *q5);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForFunctions)
{
    // Arrange
    std::shared_ptr<Expression> x1 = std::make_shared<BaseX>();
    std::shared_ptr<Expression> c = std::make_shared<Constant>(3.0);

    std::shared_ptr<Expression> f1 = std::make_shared<Sine>(x1);   // sin(x)
    std::shared_ptr<Expression> f2 = std::make_shared<Sine>(x1);   // sin(x)
    std::shared_ptr<Expression> f3 = std::make_shared<Sine>(c);    // sin(3.0)
    std::shared_ptr<Expression> f4 = std::make_shared<Cosine>(x1); // cos(x)

    // Act, Assert
    ASSERT_TRUE(*f1 == *f2);
    ASSERT_FALSE(*f1 == *f3);
    ASSERT_FALSE(*f1 == *f4);

    ASSERT_FALSE(*f1 != *f2);
    ASSERT_TRUE(*f1 != *f3);
    ASSERT_TRUE(*f1 != *f4);

    ASSERT_EQ(*f1, *f2);
    ASSERT_NE(*f1, *f3);
    ASSERT_NE(*f1, *f4);
}

#endif // TST_EQUALITY_H
