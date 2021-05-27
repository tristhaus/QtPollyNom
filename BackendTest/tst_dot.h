#ifndef TST_DOT_H
#define TST_DOT_H

#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../Backend/evaluator.h"
#include "../Backend/expression.h"
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/product.h"
#include "../Backend/sum.h"

using namespace Backend;
using namespace testing;

TEST(BackendTest, EvaluatorShallCreateGraphCorrectlyDotsShallCorrectlyDetermineBeingHitBasedOnGraph)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    // x*x
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    std::vector<std::shared_ptr<Dot>> dots;
    dots.push_back(std::make_shared<Dot>(1.05, 1.05, true));
    dots.push_back(std::make_shared<Dot>(0.0, -0.25, true));
    dots.push_back(std::make_shared<Dot>(4.0, 1.0, false));

    Evaluator evaluator(product, -10.5, 10.5, 1000.0);

    // Act
    auto graphData = evaluator.Evaluate();
    for(auto dotIterator = dots.begin(); dotIterator != dots.end(); ++dotIterator)
    {
        (*dotIterator)->CheckForHit(product, graphData);
    }

    // Assert
    EXPECT_TRUE(dots[0]->IsActive());
    EXPECT_TRUE(dots[1]->IsActive());
    EXPECT_FALSE(dots[2]->IsActive());
}

TEST(BackendTest, DotsShallCorrectlyDetermineBeingHitBasedOnAlgorithm)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    // x*x
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, baseX), Product::Factor(Product::Exponent::Positive, baseX)});

    std::vector<std::shared_ptr<Dot>> dots;
    dots.push_back(std::make_shared<Dot>(1.05, 1.05, true));
    dots.push_back(std::make_shared<Dot>(0.0, -0.25, true));
    dots.push_back(std::make_shared<Dot>(4.0, 1.0, false));

    // create empty graph/branch
    std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData;
    graphData.push_back(std::make_pair(std::vector<double>(), std::vector<double>()));

    // Act
    for(auto dotIterator = dots.begin(); dotIterator != dots.end(); ++dotIterator)
    {
        (*dotIterator)->CheckForHit(product, graphData);
    }

    // Assert
    EXPECT_TRUE(dots[0]->IsActive());
    EXPECT_TRUE(dots[1]->IsActive());
    EXPECT_FALSE(dots[2]->IsActive());
}

TEST(BackendTest, DotsShallCorrectlyHandleTrivialBorderCases)
{
    // Arrange
    auto dot = std::make_shared<Dot>(0.0, 0.0, true);

    std::vector<std::shared_ptr<Expression>> expressions;

    std::vector<bool> result;
    std::vector<bool> reference;

    auto baseX = std::make_shared<BaseX>();
    auto cp250 = std::make_shared<Constant>(250);
    auto cp500 = std::make_shared<Constant>(500);
    auto cp1000 = std::make_shared<Constant>(1000);
    auto cm1000 = std::make_shared<Constant>(-1000);

    auto top = std::make_shared<Constant>(0.25);
    expressions.push_back(top);
    reference.push_back(true);

    auto bottom = std::make_shared<Constant>(-0.25);
    expressions.push_back(bottom);
    reference.push_back(true);

    // +1000 * x - 250
    auto right = std::make_shared<Sum>(
                std::vector<Sum::Summand>
                {
                    Sum::Summand(Sum::Sign::Plus, std::make_shared<Product>(std::vector<Product::Factor>
                    {
                        Product::Factor(Product::Exponent::Positive, cp1000),
                        Product::Factor(Product::Exponent::Positive, baseX)
                    })),
                    Sum::Summand(Sum::Sign::Minus, cp250)
                });
    expressions.push_back(right);
    reference.push_back(true);

    // -1000 * x - 250
    auto left = std::make_shared<Sum>(
                std::vector<Sum::Summand>
                {
                    Sum::Summand(Sum::Sign::Plus, std::make_shared<Product>(std::vector<Product::Factor>
                    {
                        Product::Factor(Product::Exponent::Positive, cm1000),
                        Product::Factor(Product::Exponent::Positive, baseX)
                    })),
                    Sum::Summand(Sum::Sign::Minus, cp250)
                });
    expressions.push_back(left);
    reference.push_back(true);

    auto topMiss = std::make_shared<Constant>(0.3);
    expressions.push_back(topMiss);
    reference.push_back(false);

    auto bottomMiss = std::make_shared<Constant>(-0.3);
    expressions.push_back(bottomMiss);
    reference.push_back(false);

    // +1000 * x - 500
    auto rightMiss = std::make_shared<Sum>(
                std::vector<Sum::Summand>
                {
                    Sum::Summand(Sum::Sign::Plus, std::make_shared<Product>(std::vector<Product::Factor>
                    {
                        Product::Factor(Product::Exponent::Positive, cp1000),
                        Product::Factor(Product::Exponent::Positive, baseX)
                    })),
                    Sum::Summand(Sum::Sign::Minus, cp500)
                });
    expressions.push_back(rightMiss);
    reference.push_back(false);

    // -1000 * x - 500
    auto leftMiss = std::make_shared<Sum>(
                std::vector<Sum::Summand>
                {
                    Sum::Summand(Sum::Sign::Plus, std::make_shared<Product>(std::vector<Product::Factor>
                    {
                        Product::Factor(Product::Exponent::Positive, cm1000),
                        Product::Factor(Product::Exponent::Positive, baseX)
                    })),
                    Sum::Summand(Sum::Sign::Minus, cp500)
                });
    expressions.push_back(leftMiss);
    reference.push_back(false);

    // create empty graph/branch
    std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData;
    graphData.push_back(std::make_pair(std::vector<double>(), std::vector<double>()));

    // Act
    for(auto expressionIterator = expressions.begin(); expressionIterator != expressions.end(); ++expressionIterator)
    {
        dot->CheckForHit(*expressionIterator, graphData);
        result.push_back(dot->IsActive());
        dot->ResetIsActive();
    }

    // Assert
    EXPECT_EQ(result[0], reference[0]);
    EXPECT_EQ(result[1], reference[1]);
    EXPECT_EQ(result[2], reference[2]);
    EXPECT_EQ(result[3], reference[3]);
    EXPECT_EQ(result[4], reference[4]);
    EXPECT_EQ(result[5], reference[5]);
    EXPECT_EQ(result[6], reference[6]);
    EXPECT_EQ(result[7], reference[7]);
}

TEST(BackendTest, EvaluatorAndDotsShallCorrectlyHandleSingularity)
{
    // Arrange
    auto baseX = std::make_shared<BaseX>();
    auto constant = std::make_shared<Constant>(1.0);
    // 1/x
    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, constant), Product::Factor(Product::Exponent::Negative, baseX)});

    std::vector<std::shared_ptr<Dot>> dots;
    dots.push_back(std::make_shared<Dot>(10.0, 0.0, true));
    dots.push_back(std::make_shared<Dot>(0.0, 0.0, true));

    Evaluator evaluator(product, -10.5, 10.5, 1000.0);

    // Act
    auto graphData = evaluator.Evaluate();
    for(auto dotIterator = dots.begin(); dotIterator != dots.end(); ++dotIterator)
    {
        (*dotIterator)->CheckForHit(product, graphData);
    }

    // Assert
    EXPECT_TRUE(dots[0]->IsActive());
    EXPECT_FALSE(dots[1]->IsActive());
}

#endif // TST_DOT_H
