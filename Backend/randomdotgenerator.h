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

#ifndef RANDOMDOTGENERATOR_H
#define RANDOMDOTGENERATOR_H

#include "dotgenerator.h"

namespace Backend
{
    /*!
     * \class RandomDotgenerator
     * \brief The RandomDotgenerator class creates dots a grid points using a offset. It can generate 361 dots total.
     */
    class RandomDotGenerator final : public DotGenerator
    {
    private:
        unsigned short goodDots;
        unsigned short badDots;
    public:
        /*!
         * \brief Initializes a new instance.
         * \param goodDots The number of good dots to create.
         * \param badDots The number of bad dots to create.
         */
        RandomDotGenerator(unsigned short goodDots, unsigned short badDots);

        /*!
         * \reimp
         */
        virtual std::vector<std::shared_ptr<Dot>> Generate();

    private:
        static size_t GetRandomIndexUpToIncluding(size_t upper);
        static double GetRandomOffset();
    };
}


#endif // RANDOMDOTGENERATOR_H
