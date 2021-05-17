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

#ifndef CONSTANT_H
#define CONSTANT_H

#include "expression.h"

namespace Backend {

    /*!
     * \class Constant
     * \brief The Constant class represents a constant real number.
     */
    class Constant final : public Expression
    {
        Q_OBJECT

    private:
        double value;

    public:
        /*!
         * \brief Initializes a new instance holding the supplied value.
         * \param input The value to hold as a constant.
         */
        Constant(double input);
        virtual ~Constant();
        Constant(const Constant&) = delete;
        Constant(Constant&&) = delete;
        Constant& operator=(const Constant&) = delete;
        Constant& operator=(Constant&&) = delete;

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
        virtual std::optional<double> Evaluate(double) const;

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
         */
        virtual bool operator!=(const Expression &other) const;
    };

}

#endif // CONSTANT_H
