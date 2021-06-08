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

#ifndef PRODUCT_H
#define PRODUCT_H

#include <vector>
#include <memory>

#include "expression.h"

namespace Backend
{
    /*!
     * \class Product
     * \brief The Product class represents the product of the contained expressions.
     * A positive or negative exponent of 1 is applied to the expressions, depending
     * on whether multiplication or division is intended.
     *
     * Note that the Product supports commutativity, but not associativity (or distributivity for that matter)
     * when checking for (in)equality.
     */
    class Product final : public Expression
    {
    public:
        /*!
         * \enum Exponent
         * \brief The Exponent enum represents the valid choices for exponents for a \ref Factor in a \ref Product
         *
         * \value Plus The positive exponent, indicating multiplication.
         * \value Minus The negative exponent, indicating division.
         */
        enum Exponent
        {
            Positive = 2,
            Negative = 3
        };

        /*!
         * \struct Factor
         * \brief The Factor struct collects a \ref Exponent and an expression.
         */
        struct Factor
        {
        public:
            Product::Exponent exponent;
            std::shared_ptr<Expression> expression;

            /*!
             * \brief Initializes a new instance holding the supplied \ref Exponent and expression.
             * \param sign The exponent to apply.
             * \param expression The expression contained.
             */
            Factor(Product::Exponent exponent, std::shared_ptr<Expression> expression);

            /*!
             * \brief Equality operator for the factor, checking exponent and expression.
             * \param other The instance to compare to.
             * \return A value indicating equality.
             */
            bool operator==(const Factor &other) const;

            /*!
             * \brief Inequality operator for the factor, checking exponent and expression.
             * \param other The instance to compare to.
             * \return A value indicating inequality.
             */
            bool operator!=(const Factor &other) const;
        };

    private:
        std::vector<Factor> factors;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied factors.
         * \param factors The factors making up the product.
         */
        Product(std::vector<Factor> factors);
        virtual ~Product();
        Product(const Product&) = delete;
        Product(Product&&) = delete;
        Product& operator=(const Product&) = delete;
        Product& operator=(Product&&) = delete;

        /*!
         * \reimp
         */
        virtual int GetLevel() const;

        /*!
         * \reimp
         */
        virtual bool IsMonadic() const;

        /*!
         * \reimp
         */
        virtual std::optional<double> Evaluate(double input) const;

        /*!
         * \reimp
         */
        virtual std::optional<std::string> Print() const;

        /*!
         * \reimp
         */
        virtual bool operator==(const Expression &other) const;

        /*!
         * \reimp
         */virtual bool operator!=(const Expression &other) const;
    };
}

#endif // PRODUCT_H
