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

#ifndef SUM_H
#define SUM_H

#include <vector>
#include <memory>

#include "expression.h"

namespace Backend
{
    /*!
     * \class Sum
     * \brief The Sum class represents the sum of the contained expressions.
     * The expressions are prefixed with a positive or a negative sign.
     *
     * Note that the Sum supports commutativity, but not associativity (or distributivity for that matter)
     * when checking for (in)equality.
     */
    class Sum final : public Expression
    {
    public:
        /*!
         * \enum Sign
         * \brief The Sign enum represents the valid choices for signs for a \ref Summand in a \ref Sum
         *
         * \value Plus The positive sign.
         * \value Minus The negative sign.
         */
        enum Sign
        {
            Plus = 0,
            Minus = 1
        };

        /*!
         * \struct Summand
         * \brief The Summand struct collects a \ref Sign and an expression.
         */
        struct Summand
        {
        public:
            Sum::Sign sign;
            std::shared_ptr<Expression> expression;

            /*!
             * \brief Initializes a new instance holding the supplied \ref Sign and expression.
             * \param sign The sign to apply.
             * \param expression The expression contained.
             */
            Summand(Sum::Sign sign, std::shared_ptr<Expression> expression);

            /*!
             * \brief Equality operator for the summand, checking sign and expression.
             * \param other The instance to compare to.
             * \return A value indicating equality.
             */
            bool operator==(const Summand &other) const;

            /*!
             * \brief Inequality operator for the summand, checking sign and expression.
             * \param other The instance to compare to.
             * \return A value indicating inequality.
             */
            bool operator!=(const Summand &other) const;
        };

    private:
        std::vector<Summand> summands;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied \ref Summand instances.
         * \param summands The collection of summands to hold in the sum.
         */
        Sum(std::vector<Summand> summands);
        virtual ~Sum();
        Sum(const Sum&) = delete;
        Sum(Sum&&) = delete;
        Sum& operator=(const Sum&) = delete;
        Sum& operator=(Sum&&) = delete;

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

#endif // SUM_H
