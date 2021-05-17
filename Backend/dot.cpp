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

#include "dot.h"
#include <utility>

Dot::Dot(double x, double y, bool goodDot)
    : x(x), y(y), radius(0.25),
      isActive(false), isGood(goodDot)
{
}

void Dot::SetIsActive(bool active)
{
    isActive = active;
}

std::pair<double, double> Dot::GetCoordinates() const
{
    return std::pair(x, y);
}

double Dot::GetRadius() const
{
    return radius;
}

bool Dot::IsActive() const
{
    return isActive;
}

bool Dot::IsGood() const
{
    return isGood;
}
