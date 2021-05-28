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

#ifndef BASEX_H
#define BASEX_H

#include "expression.h"

namespace Backend {

    /*!
     * \class BaseX
     * \brief The BaseX class represents the independent (x) variable.
     */
    class BaseX final : public Expression
    {
    public:
        /*!
         * \brief Initializes a new instance.
         */
        BaseX();
        virtual ~BaseX();
        BaseX(const BaseX&) = delete;
        BaseX(BaseX&&) = delete;
        BaseX& operator=(const BaseX&) = delete;
        BaseX& operator=(BaseX&&) = delete;

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

#endif // BASEX_H
