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
 * that creates the same dots over and over, depending on configuration.
 */
class FixedDotGenerator final : public DotGenerator
{
private:
    unsigned short period;
    unsigned short state;

public:
    /*!
     * \brief Initializes a new instance.
     * \param period The number of Generate() calls until the dots begin to repeat.
     *        Current maximum is 2.
     */
    FixedDotGenerator(unsigned short period = 1);

    /*!
     * \reimp
     */
    virtual std::vector<std::shared_ptr<Dot>> Generate();

private:
    std::vector<std::shared_ptr<Dot>> GenerateSet0();
    std::vector<std::shared_ptr<Dot>> GenerateSet1();
};

#endif // FIXEDDOTGENERATOR_H
