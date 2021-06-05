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

#ifndef POWER_H
#define POWER_H

#include "expression.h"
#include <memory>

namespace Backend
{

class Power final : public Expression
{
private:
    std::shared_ptr<Expression> base;
    std::shared_ptr<Expression> exponent;
public:
    Power(std::shared_ptr<Expression> base, std::shared_ptr<Expression> exponent);
    virtual ~Power();
    Power(const Power&) = delete;
    Power(Power&&) = delete;
    Power& operator=(const Power&) = delete;
    Power& operator=(Power&&) = delete;

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

#endif // POWER_H
