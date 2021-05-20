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

#ifndef TST_PRODUCT_H
#define TST_PRODUCT_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, ProductShallEvaluateCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c1 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Constant> c2 = std::make_shared<Constant>(2.0);

    std::vector<Sum::Summand> summands;
    summands.push_back(Sum::Summand(Sum::Sign::Plus, x));
    summands.push_back(Sum::Summand(Sum::Sign::Minus, c1));

    std::shared_ptr<Sum> sum = std::make_shared<Sum>(summands);

    Product product({Product::Factor(Product::Exponent::Positive, x), Product::Factor(Product::Exponent::Negative, c2), Product::Factor(Product::Exponent::Positive, sum)});

    // Act
    auto result1 = product.Evaluate(0.0);
    auto result2 = product.Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(result1.value(), 0.0);
    EXPECT_DOUBLE_EQ(result2.value(), 16.875);
}

TEST(BackendTest, ProductShallEvaluateUndefinedDivision)
{
    // Arrange
    std::shared_ptr<Constant> c = std::make_shared<Constant>(1.0);
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();

    Product product({Product::Factor(Product::Exponent::Positive, c), Product::Factor(Product::Exponent::Negative, x)});

    // Act
    auto result1 = product.Evaluate(0.0);
    auto result2 = product.Evaluate(-1.0e-10);
    auto result3 = product.Evaluate(-1.0e-9);

    // Assert
    EXPECT_FALSE(result1.has_value());
    EXPECT_FALSE(result2.has_value());
    ASSERT_TRUE(result3.has_value());

    EXPECT_DOUBLE_EQ(result3.value(), -1.0e9);
}

TEST(BackendTest, ProductShallPrintCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c1 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Constant> c2 = std::make_shared<Constant>(2.0);

    std::vector<Sum::Summand> summands;
    summands.push_back(Sum::Summand(Sum::Sign::Plus, x));
    summands.push_back(Sum::Summand(Sum::Sign::Minus, c1));

    std::shared_ptr<Sum> sum = std::make_shared<Sum>(summands);

    Product product({Product::Factor(Product::Exponent::Positive, x), Product::Factor(Product::Exponent::Negative, c2), Product::Factor(Product::Exponent::Positive, sum)});

    // Act
    auto result1 = product.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());

    EXPECT_STREQ(result1.value().c_str(), "x/2.000000*(x-3.000000)");
}

#endif // TST_PRODUCT_H
