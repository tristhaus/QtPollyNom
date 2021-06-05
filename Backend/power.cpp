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

#include "power.h"

namespace Backend
{

Power::Power(std::shared_ptr<Expression> base, std::shared_ptr<Expression> exponent)
 : base(base),
   exponent(exponent)
{
}

Power::~Power()
{
}

int Power::GetLevel() const
{
    return 3;
}

bool Power::IsMonadic() const
{
    return false;
}

std::optional<double> Power::Evaluate(double input) const
{
    return 0.0; //todo
}

std::optional<std::string> Power::Print() const
{
    auto baseOptional = base->Print();
    auto exponentOptional = exponent->Print();

    if(!baseOptional.has_value() || !exponentOptional.has_value())
    {
        return {};
    }

    auto baseString = base->IsMonadic()
            ? baseOptional.value()
            : "(" + baseOptional.value() + ")";

    auto exponentString = exponent->IsMonadic()
            ? exponentOptional.value()
            : "(" + exponentOptional.value() + ")";

    return std::string(baseString + "^" + exponentString);
}

bool Power::operator==(const Expression& other) const
{
    if (const Power * b = dynamic_cast<const Power*>(&other))
    {
        if(b == nullptr)
        {
            return false;
        }

        return *(this->base) == *(b->base) && *(this->exponent) == *(b->exponent);
    }
    else
    {
        return false;
    }
}

bool Power::operator!=(const Expression& other) const
{
    return !(*this == other);
}

}
