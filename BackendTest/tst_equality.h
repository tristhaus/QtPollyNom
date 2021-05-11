#ifndef TST_EQUALITY_H
#define TST_EQUALITY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"

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


#endif // TST_EQUALITY_H
