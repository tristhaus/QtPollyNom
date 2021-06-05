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

#ifndef TST_POWER_H
#define TST_POWER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/power.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"

using namespace testing;
using namespace Backend;

TEST(BackendTest, PowerShallPrintCorrectly)
{
    // Arrange
    std::shared_ptr<BaseX> x = std::make_shared<BaseX>();
    std::shared_ptr<Constant> c1 = std::make_shared<Constant>(3.0);
    std::shared_ptr<Constant> c2 = std::make_shared<Constant>(2.0);

    Power power1(x, c1);

    std::vector<Sum::Summand> summands;
    summands.push_back(Sum::Summand(Sum::Sign::Plus, x));
    summands.push_back(Sum::Summand(Sum::Sign::Minus, c1));

    std::shared_ptr<Sum> sum = std::make_shared<Sum>(summands);

    std::vector<Product::Factor> factors;
    factors.push_back(Product::Factor(Product::Exponent::Positive, x));
    factors.push_back(Product::Factor(Product::Exponent::Negative, c2));

    std::shared_ptr<Product> product = std::make_shared<Product>(factors);

    Power power2(sum, product);

    // Act
    auto result1 = power1.Print();
    auto result2 = power2.Print();

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_STREQ(result1.value().c_str(), "x^3.000000");
    EXPECT_STREQ(result2.value().c_str(), "(x-3.000000)^(x/2.000000)");
}

#endif // TST_POWER_H
