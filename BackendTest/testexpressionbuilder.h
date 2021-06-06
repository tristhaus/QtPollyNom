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

#ifndef TESTEXPRESSIONBUILDER_H
#define TESTEXPRESSIONBUILDER_H

#include <memory>
#include "../Backend/constant.h"
#include "../Backend/basex.h"
#include "../Backend/expression.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"
#include "../Backend/power.h"
#include "../Backend/parser.h"

using namespace Backend;

/*!
 * \brief The TestExpressionBuilder class creates expressions for testing purposes.
 * It is abstract.
 */
class TestExpressionBuilder
{
public:
    TestExpressionBuilder() = delete;

    /*!
     * \brief Builds the expression representing 2.0*x.
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build01();

    /*!
     * \brief Builds the expression representing 2.0*x^3.0/(x-2^x).
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build02();

    /*!
     * \brief Builds the expression representing 2.0*(x+1.0).
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build03();

    /*!
     * \brief Builds the expression representing (x+1.0)^2.0.
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build04();

    /*!
     * \brief Builds the expression representing (x+1.0)^(x/3.0).
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build05();

    /*!
     * \brief Builds the expression representing x-1+2-3.
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build06();

    /*!
     * \brief Builds the expression representing x+1-2+3.
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build07();

    /*!
     * \brief Builds the expression representing x+1-4+7.
     * \return An expression instance.
     */
    static std::shared_ptr<Expression> Build08();
};

#endif // TESTEXPRESSIONBUILDER_H
