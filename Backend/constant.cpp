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

#include "constant.h"

namespace Backend {

    Constant::Constant(double input) : value(input)
    {
    }

    Constant::~Constant()
    {
    }

    int Constant::GetLevel() const
    {
        return 0;
    }

    bool Constant::IsMonadic() const
    {
        return true;
    }

    std::optional<double> Constant::Evaluate(double) const
    {
        return this->value;
    }

    std::optional<std::wstring> Constant::Print() const
    {
        return std::to_wstring(this->value);
    }

    bool Constant::operator==(const Expression &other) const
    {
        if (const Constant * b = dynamic_cast<const Constant*>(&other))
        {
            return b != NULL && this->value == b->value;
        }
        else
        {
            return false;
        }
    }

    bool Constant::operator!=(const Expression &other) const
    {
        return !this->operator==(other);
    }

}
