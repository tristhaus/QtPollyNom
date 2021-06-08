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
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/product.h"
#include "../Backend/power.h"

using namespace Backend;
using namespace testing;

TEST(BackendTest, EvaluatorShallThrowOnBadCallSequence)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    Evaluator evaluator1(product, -10.5, 10.5, 1000.0);
    Evaluator evaluator2(product, -10.5, 10.5, 1000.0);

    // Act, Assert
    try
    {
        evaluator1.AddPointToCurrentBranchAt(2.0);
        evaluator1.Evaluate();
        FAIL();
    }
    catch (std::exception&)
    {
        SUCCEED();
    }

    try
    {
        evaluator2.Evaluate();
        evaluator2.AddPointToCurrentBranchAt(2.0);
        FAIL();
    }
    catch (std::exception&)
    {
        SUCCEED();
    }
}

TEST(BackendTest, EvaluatorShallCorrectlySortPoints)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    Evaluator evaluator(product, -10.5, 10.5, 1000.0);

    // Act
    evaluator.AddPointToCurrentBranchAt(2.0);
    evaluator.AddPointToCurrentBranchAt(1.0);
    evaluator.AddPointToCurrentBranchAt(1.5);
    evaluator.AddPointToCurrentBranchAt(3.0);

    auto result = evaluator.GetGraph().back();

    // Assert
    double compare = result.first.front();
    for(unsigned long long i=1; i<result.first.size(); ++i)
    {
        auto current = result.first[i];
        ASSERT_LT(compare, current);
        compare = current;
    }

    EXPECT_DOUBLE_EQ(1.0, result.second[0]);
    EXPECT_DOUBLE_EQ(2.25, result.second[1]);
    EXPECT_DOUBLE_EQ(4.0, result.second[2]);
    EXPECT_DOUBLE_EQ(9.0, result.second[3]);
}

TEST(BackendTest, ForXSquaredEvaluatorShallCreateOneList)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    Evaluator evaluator(product, -10.5, 10.5, 1000.0);

    // Act
    auto graph = evaluator.Evaluate();

    // Assert
    ASSERT_EQ(1, graph.size());
    EXPECT_GE(graph[0].first.size(), 1);
    EXPECT_GE(graph[0].second.size(), 1);
}

TEST(BackendTest, ForOneOverXEvaluatorShallCreateTwoLists)
{
    // Arrange
    auto constant = std::make_shared<Constant>(1.0);
    auto baseX = std::make_shared<BaseX>();
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant), Product::Factor(Product::Exponent::Negative, baseX)});

    Evaluator evaluator(product, -10.5, 10.5, 1000.0);

    // Act
    auto graph = evaluator.Evaluate();

    // Assert
    ASSERT_EQ(2, graph.size() );
    EXPECT_GE(graph[0].first.size(), 1);
    EXPECT_GE(graph[0].second.size(), 1);
    EXPECT_GE(graph[1].first.size(), 1);
    EXPECT_GE(graph[1].second.size(), 1);
}

TEST(BackendTest, EvaluatorShallNotCreateEmptyBranchesAlongValidOnes)
{
    // Arrange
    auto constant = std::make_shared<Constant>(2.0);
    auto baseX = std::make_shared<BaseX>();
    auto power = std::make_shared<Power>(constant, baseX);

    Evaluator evaluator(power, -10.5, 10.5, 1000.0);

    // Act
    auto graph = evaluator.Evaluate();

    // Assert
    ASSERT_EQ(1, graph.size());
    EXPECT_GE(graph[0].first.size(), 1);
    EXPECT_GE(graph[0].second.size(), 1);
}

TEST(BackendTest, EvaluatorShallCreateOneEmptyBranchIfFunctionDomainDisjoint)
{
    // Arrange
    auto c1 = std::make_shared<Constant>(-100.0);
    auto c2 = std::make_shared<Constant>(1/2);
    auto power = std::make_shared<Power>(c1, c2);

    Evaluator evaluator(power, -10.5, 10.5, 1000.0);

    // Act
    auto graph = evaluator.Evaluate();

    // Assert
    ASSERT_EQ(1, graph.size());
    EXPECT_GE(graph[0].first.size(), 0);
    EXPECT_GE(graph[0].second.size(), 0);
}

#endif // TST_EVALUATOR_H
