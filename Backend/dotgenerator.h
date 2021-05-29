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

#ifndef DOTGENERATOR_H
#define DOTGENERATOR_H

#include "dot.h"

namespace Backend
{
    /*!
     * \brief The DotGenerator class forms the base for ways to generate dots.
     *
     * It is abstract.
     */
    class DotGenerator
    {
    public:
        explicit DotGenerator()
        {
        }
        virtual ~DotGenerator()
        {
        }
        DotGenerator(const DotGenerator&) = delete;
        DotGenerator(DotGenerator&&) = delete;
        DotGenerator& operator=(const DotGenerator&) = delete;
        DotGenerator& operator=(DotGenerator&&) = delete;

        /*!
         * \brief Perform the generation of dots.
         * \return A collection of good and/or bad dots.
         */
        virtual std::vector<std::shared_ptr<Dot>> Generate() = 0;
    };
}

#endif // DOTGENERATOR_H
