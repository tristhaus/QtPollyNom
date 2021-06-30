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

#include "subsetgenerator.h"
#include <bitset>

unsigned long long ipow(unsigned long long base, unsigned long long exp)
{
    unsigned long long result = 1ULL;
    while(exp)
    {
        if(exp & 1)
        {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }

    return result;
}

SubsetGenerator::SubsetGenerator(std::string input) :
    input(input),
    index(1),
    limitIndex(ipow(2, input.length()))
{
    if(input.length() > sizeof(unsigned long long)*8)
    {
        throw std::exception("programmer mistake: SubsetGenerator was passed a too long input");
    }
}

bool SubsetGenerator::HasNext() const
{
    return index < limitIndex;
}

std::string SubsetGenerator::GetNext()
{
    std::string retval("");
    std::bitset<sizeof(unsigned long long)*8> b(index);
    const auto length = input.length();

    for(size_t i = 0; i < length; ++i)
    {
        if(b[i])
        {
            retval += input[i];
        }
    }

    ++index;
    return retval;
}
