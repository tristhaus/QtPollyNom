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

#include "testexpressionbuilder.h"

std::shared_ptr<Expression> TestExpressionBuilder::Build01()
{
    // 2.0*x
    auto c = std::make_shared<Constant>(2.0);
    auto x = std::make_shared<BaseX>();

    auto product = std::make_shared<Product>(std::vector<Product::Factor>{Product::Factor(Product::Exponent::Positive, c), Product::Factor(Product::Exponent::Positive, x)});
    return product;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build02()
{
    // 2.0*x^3.0/(x-2^x)
    auto c1 = std::make_shared<Constant>(2.0);
    auto c2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();

    auto factor2 = std::make_shared<Power>(x, c2);

    auto power = std::make_shared<Power>(c1, x);

    auto factor3 = std::make_shared<Sum>(std::vector<Sum::Summand>
                                         {
                                             Sum::Summand(Sum::Sign::Plus, x),
                                             Sum::Summand(Sum::Sign::Minus, power)
                                         });

    auto product = std::make_shared<Product>(std::vector<Product::Factor>
                                             {
                                                 Product::Factor(Product::Exponent::Positive, c1),
                                                 Product::Factor(Product::Exponent::Positive, factor2),
                                                 Product::Factor(Product::Exponent::Negative, factor3)
                                             });

    return product;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build03()
{
    // 2.0*(x+1.0)
    auto c1 = std::make_shared<Constant>(2.0);
    auto c2 = std::make_shared<Constant>(1.0);
    auto x = std::make_shared<BaseX>();

    auto factor2 = std::make_shared<Sum>(std::vector<Sum::Summand>
                                         {
                                             Sum::Summand(Sum::Sign::Plus, x),
                                             Sum::Summand(Sum::Sign::Plus, c2)
                                         });

    auto product = std::make_shared<Product>(std::vector<Product::Factor>
                                             {
                                                 Product::Factor(Product::Exponent::Positive, c1),
                                                 Product::Factor(Product::Exponent::Positive, factor2)
                                             });

    return product;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build04()
{
    // (x+1.0)^2.0
    auto c1 = std::make_shared<Constant>(2.0);
    auto c2 = std::make_shared<Constant>(1.0);
    auto x = std::make_shared<BaseX>();

    auto base = std::make_shared<Sum>(std::vector<Sum::Summand>
                                      {
                                          Sum::Summand(Sum::Sign::Plus, x),
                                          Sum::Summand(Sum::Sign::Plus, c2)
                                      });

    auto power = std::make_shared<Power>(base, c1);

    return power;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build05()
{
    // (x+1.0)^(x/3.0)
    auto c1 = std::make_shared<Constant>(1.0);
    auto c2 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();

    auto base = std::make_shared<Sum>(std::vector<Sum::Summand>
                                      {
                                          Sum::Summand(Sum::Sign::Plus, x),
                                          Sum::Summand(Sum::Sign::Plus, c1)
                                      });

    auto exponent = std::make_shared<Product>(std::vector<Product::Factor>
                                              {
                                                  Product::Factor(Product::Exponent::Positive, x),
                                                  Product::Factor(Product::Exponent::Negative, c2)
                                              });

    auto power = std::make_shared<Power>(base, exponent);

    return power;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build06()
{
    // x-1+2-3
    auto c1 = std::make_shared<Constant>(1.0);
    auto c2 = std::make_shared<Constant>(2.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();

    auto sum = std::make_shared<Sum>(std::vector<Sum::Summand>
                                      {
                                          Sum::Summand(Sum::Sign::Plus, x),
                                          Sum::Summand(Sum::Sign::Minus, c1),
                                          Sum::Summand(Sum::Sign::Plus, c2),
                                          Sum::Summand(Sum::Sign::Minus, c3)
                                      });

    return sum;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build07()
{
    // x+1-2+3
    auto c1 = std::make_shared<Constant>(1.0);
    auto c2 = std::make_shared<Constant>(2.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto x = std::make_shared<BaseX>();

    auto sum = std::make_shared<Sum>(std::vector<Sum::Summand>
                                      {
                                          Sum::Summand(Sum::Sign::Plus, x),
                                          Sum::Summand(Sum::Sign::Plus, c1),
                                          Sum::Summand(Sum::Sign::Minus, c2),
                                          Sum::Summand(Sum::Sign::Plus, c3)
                                      });

    return sum;
}

std::shared_ptr<Expression> TestExpressionBuilder::Build08()
{
    // x+1-4+7
    auto c1 = std::make_shared<Constant>(1.0);
    auto c2 = std::make_shared<Constant>(4.0);
    auto c3 = std::make_shared<Constant>(7.0);
    auto x = std::make_shared<BaseX>();

    auto sum = std::make_shared<Sum>(std::vector<Sum::Summand>
                                      {
                                          Sum::Summand(Sum::Sign::Plus, x),
                                          Sum::Summand(Sum::Sign::Plus, c1),
                                          Sum::Summand(Sum::Sign::Minus, c2),
                                          Sum::Summand(Sum::Sign::Plus, c3)
                                      });

    return sum;
}
