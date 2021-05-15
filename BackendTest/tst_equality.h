#ifndef TST_EQUALITY_H
#define TST_EQUALITY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/sum.h"

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

    auto x1Summand = Sum::Summand(Sum::Sign::Plus, x1);
    auto x2Summand = Sum::Summand(Sum::Sign::Minus, x2);

    Sum s1 = Sum({x1Summand, x1Summand});
    Sum s2 = Sum({x1Summand, x1Summand});
    Sum s3 = Sum({x1Summand, x2Summand});
    Sum s4 = Sum({x1Summand, x1Summand, x1Summand});
    Sum s5 = Sum({x2Summand, x1Summand});

    // Act, Assert
    ASSERT_TRUE(s1 == s2);
    ASSERT_FALSE(s1 == s3);
    ASSERT_FALSE(s1 == s4);
    ASSERT_TRUE(s3 == s5);

    ASSERT_FALSE(s1 != s2);
    ASSERT_TRUE(s1 != s3);
    ASSERT_TRUE(s1 != s4);
    ASSERT_FALSE(s3 != s5);

    ASSERT_EQ(s1, s2);
    ASSERT_NE(s1, s3);
    ASSERT_NE(s1, s4);
    ASSERT_EQ(s3, s5);
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

    auto x1Summand = Sum::Summand(Sum::Sign::Plus, x1);
    auto x2Summand = Sum::Summand(Sum::Sign::Minus, x2);
    auto cSummand = Sum::Summand(Sum::Sign::Minus, c);

    std::shared_ptr<Expression> s1 = std::make_shared<Sum>(std::vector<Sum::Summand>({x1Summand, x1Summand}));
    std::shared_ptr<Expression> s2 = std::make_shared<Sum>(std::vector<Sum::Summand>({x1Summand, x2Summand}));

    auto s1Summand = Sum::Summand(Sum::Sign::Plus, s1);
    auto s2Summand = Sum::Summand(Sum::Sign::Minus, s2);

    std::shared_ptr<Expression> s3 = std::make_shared<Sum>(std::vector<Sum::Summand>({cSummand, s1Summand, s2Summand}));
    std::shared_ptr<Expression> s4 = std::make_shared<Sum>(std::vector<Sum::Summand>({s2Summand, s1Summand, cSummand}));

    ASSERT_TRUE(*s3 == *s4);
    ASSERT_FALSE(*s3 != *s4);

    ASSERT_EQ(*s3, *s4);
}

#endif // TST_EQUALITY_H
