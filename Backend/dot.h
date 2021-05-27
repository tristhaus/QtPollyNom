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

#include <vector>
#include <memory>

#include "expression.h"

namespace Backend {

    /*!
     * \class Dot
     * \brief The Dot class represents a circular target zone that a graph may or may not hit.
     */
    class Dot final
    {
    private:
        double x;
        double y;
        double radius;
        bool isActive;
        bool isGood;

    public:
        /*!
         * \brief Initializes a new instance.
         * \param x The x coordinate of the dot center.
         * \param y The y coordinate of the dot center.
         * \param isGoodDot Value indicating whether the dot is a good dot (i.e. should be hit).
         */
        Dot(double x, double y, bool isGoodDot = true);
        ~Dot() = default;
        Dot(const Dot&) = default;
        Dot(Dot&&) = default;
        Dot& operator=(const Dot&) = default;
        Dot& operator=(Dot&&) = default;

        /*!
         * \brief Gets the coordinates of the center of the dot as an ordered pair.
         * \return A pair comprising the x and y coordinates, in that order.
         */
        std::pair<double, double> GetCoordinates() const;

        /*!
         * \brief Gets the radius of the dot in graph units.
         * \return The radius.
         */
        double GetRadius() const;

        /*!
         * \brief Gets a value indicating whether the dot is currently hit.
         * \return true if the dot is currently hit.
         */
        bool IsActive() const;

        /*!
         * \brief Gets a value indicating whether the dot is a good dot.
         * \return true if the dot is a good dot.
         */
        bool IsGood() const;

        /*!
         * \brief Reset resets the \ref IsActive flag to false.
         */
        void ResetIsActive();

        /*!
         * \brief CheckForHit checks whether the dot is hit by the current expression and its graph data.
         * \param expression The current expression.
         * \param graphData The otherwise created graph data for the expression.
         */
        void CheckForHit(const std::shared_ptr<Expression> expression, const std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData);

    private:
        /*!
         * \brief GetRandom provides a random number between 0.0 and 1.0.
         * \return A random number between 0.0 and 1.0.
         */
        static double GetRandom();

        /*!
         * \brief Sets the IsActive value.
         * \param isActive Value indicating whether the dot is active.
         */
        void SetIsActive(bool isActive);
    };

}

#endif // DOT_H
