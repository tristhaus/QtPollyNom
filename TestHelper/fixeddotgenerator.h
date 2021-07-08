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

#ifndef FIXEDDOTGENERATOR_H
#define FIXEDDOTGENERATOR_H

#include "../Backend/dotgenerator.h"

using namespace Backend;

/*!
 * \brief The FixedDotGenerator class is a dot generator
 * that always creates the same dots.
 */
class FixedDotGenerator final : public DotGenerator
{
public:
    /*!
     * \brief Initializes a new instance.
     */
    FixedDotGenerator() = default;

    /*!
     * \reimp
     */
    virtual std::vector<std::shared_ptr<Dot>> Generate();
};

#endif // FIXEDDOTGENERATOR_H
