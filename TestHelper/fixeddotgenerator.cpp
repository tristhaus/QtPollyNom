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

#include "fixeddotgenerator.h"

FixedDotGenerator::FixedDotGenerator(unsigned short period)
    : period(period),
      state(0)
{
}

// the basic set
std::vector<std::shared_ptr<Dot>> FixedDotGenerator::GenerateSet0()
{
    std::vector<std::shared_ptr<Dot>> retval;

    retval.push_back(std::make_shared<Dot>(1.0, 1.0, true));
    retval.push_back(std::make_shared<Dot>(-8.0, -0.25, true));
    retval.push_back(std::make_shared<Dot>(-4.0, 0.35, true));
    retval.push_back(std::make_shared<Dot>(5.0, -5.0, true));
    retval.push_back(std::make_shared<Dot>(2.5, 5.0, false));

    return retval;
}

// like Set0, but mirrored about the origin
std::vector<std::shared_ptr<Dot>> FixedDotGenerator::GenerateSet1()
{
    std::vector<std::shared_ptr<Dot>> retval;

    retval.push_back(std::make_shared<Dot>(-1.0, -1.0, true));
    retval.push_back(std::make_shared<Dot>(8.0, 0.25, true));
    retval.push_back(std::make_shared<Dot>(4.0, -0.35, true));
    retval.push_back(std::make_shared<Dot>(-5.0, 5.0, true));
    retval.push_back(std::make_shared<Dot>(-2.5, -5.0, false));

    return retval;
}

std::vector<std::shared_ptr<Dot>> FixedDotGenerator::Generate()
{
    std::vector<std::shared_ptr<Dot>> retval(0);

    switch (state)
    {
    case 0:
        retval = GenerateSet0();
        break;
    case 1:
        retval = GenerateSet1();
        break;
    default:
        throw std::exception("programming mistake: period used in fixeddotgenerator is too large");
    }

    ++state;
    state %= period;

    return retval;
}
