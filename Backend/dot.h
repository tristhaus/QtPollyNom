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

#ifndef DOT_H
#define DOT_H

#include <utility>

class Dot
{
private:
    double x;
    double y;
    double radius;
    bool isActive;
    bool isGood;

public:
    Dot(double x, double y, bool goodDot = true);
    ~Dot() = default;
    Dot(const Dot&) = default;
    Dot(Dot&&) = default;
    Dot& operator=(const Dot&) = default;
    Dot& operator=(Dot&&) = default;

    void SetIsActive(bool active);

    std::pair<double, double> GetCoordinates() const;
    double GetRadius() const;
    bool IsActive() const;
    bool IsGood() const;
};

#endif // DOT_H
