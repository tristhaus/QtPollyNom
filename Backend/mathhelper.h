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

#ifndef MATHHELPER_H
#define MATHHELPER_H

namespace Backend
{
    /*!
     * \brief SquareDistance calculates the square of the Cartesian distance between two points.
     * \param x1 x-coordinate of first point.
     * \param y1 y-coordinate of first point.
     * \param x2 x-coordinate of second point.
     * \param y2 y-coordinate of second point.
     * \return The square of the Cartesian distance between the points.
     */
    double SquareDistance(double x1, double y1, double x2, double y2);
}

#endif // MATHHELPER_H
