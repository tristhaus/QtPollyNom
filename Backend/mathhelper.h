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
