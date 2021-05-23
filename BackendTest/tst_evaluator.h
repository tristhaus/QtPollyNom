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

#ifndef TST_EVALUATOR_H
#define TST_EVALUATOR_H

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/evaluator.h"
#include "../Backend/expression.h"
#include "../Backend/basex.h"
#include "../Backend/product.h"

using namespace Backend;
using namespace testing;

TEST(BackendTest, EvaluatorShallCorrectlySortPoints)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    Evaluator evaluator(product);

    // Act
    evaluator.AddPointAt(2.0);
    evaluator.AddPointAt(1.0);
    evaluator.AddPointAt(1.5);
    evaluator.AddPointAt(3.0);

    auto result = evaluator.Evaluate();

    // Assert
    double compare = result.first.front();
    for(unsigned long long i=1; i<result.first.size(); ++i)
    {
        auto current = result.first[i];
        ASSERT_LT(compare, current);
        compare = current;
    }

    EXPECT_DOUBLE_EQ(result.second[0], 1.0);
    EXPECT_DOUBLE_EQ(result.second[1], 2.25);
    EXPECT_DOUBLE_EQ(result.second[2], 4.0);
    EXPECT_DOUBLE_EQ(result.second[3], 9.0);
}

#endif // TST_EVALUATOR_H
