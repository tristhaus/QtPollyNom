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

#include <random>
#include "randomdotgenerator.h"

namespace Backend
{
    RandomDotGenerator::RandomDotGenerator(unsigned short goodDots, unsigned short badDots)
        : goodDots(goodDots),
          badDots(badDots)
    {
    }

    std::vector<std::shared_ptr<Dot>> RandomDotGenerator::Generate()
    {
        const int maxLine = 19;
        const int maxPositions = maxLine*maxLine;
        const double downCorrection = 9.0;

        if(goodDots + badDots > maxPositions)
        {
            throw std::exception("programmer mistake: cannot use more than maxPositions dots");
        }

        std::vector<int> allPositions;
        for (int i = 0; i<maxPositions; ++i)
        {
            allPositions.push_back(i);
        }

        std::vector<std::shared_ptr<Dot>> retval;

        auto createDot = [&](const bool kind)
        {
            auto index = RandomDotGenerator::GetRandomIndexUpToIncluding(allPositions.size() - 1);
            int position = allPositions[index];
            allPositions.erase(allPositions.begin() + index);

            auto x = static_cast<double>(position/maxLine) - downCorrection + RandomDotGenerator::GetRandomOffset();
            auto y = static_cast<double>(position%maxLine) - downCorrection + RandomDotGenerator::GetRandomOffset();

            retval.push_back(std::make_shared<Dot>(x, y, kind));
        };

        for (int i = 0; i<goodDots; ++i)
        {
            const bool goodDot = true;

            createDot(goodDot);
        }

        for (int i = 0; i<badDots; ++i)
        {
            const bool goodDot = false;

            createDot(goodDot);
        }

        return retval;
    }

    /* static class member */ size_t RandomDotGenerator::GetRandomIndexUpToIncluding(size_t upper)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, upper);
        return dis(gen);
    }

    /* static class member */ double RandomDotGenerator::GetRandomOffset()
    {
        const double maxOffset = 0.15;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution  dis(-maxOffset, maxOffset);
        return dis(gen);
    }
}
