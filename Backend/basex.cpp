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

#include "basex.h"

namespace Backend {

    BaseX::BaseX()
    {
    }

    BaseX::~BaseX()
    {
    }

    int BaseX::GetLevel() const
    {
        return 0;
    }

    bool BaseX::IsMonadic() const
    {
        return true;
    }

    std::optional<double> BaseX::Evaluate(double input) const
    {
        return input;
    }

    std::optional<std::wstring> BaseX::Print() const
    {
        return std::wstring(L"x");
    }

    bool BaseX::operator==(const Expression &other) const
    {
        if (const BaseX * b = dynamic_cast<const BaseX*>(&other))
        {
            return b != NULL;
        }
        else
        {
            return false;
        }
    }

    bool BaseX::operator!=(const Expression &other) const
    {
        return !this->operator==(other);
    }

}
