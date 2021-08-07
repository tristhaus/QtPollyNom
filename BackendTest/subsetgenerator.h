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

#ifndef SUBSETGENERATOR_H
#define SUBSETGENERATOR_H

#include <string>

/*!
 * \brief The SubsetGenerator class creates from the input all strings
 *        that omit one or more characters from the input.
 *
 * For technical reasons, the input cannot be too long.
 */
class SubsetGenerator final
{
private:
    const std::wstring input;
    unsigned long long index;
    const unsigned long long limitIndex;
public:
    /*!
     * \brief Initializes a new instance.
     * \param input The string to modify.
     */
    SubsetGenerator(std::wstring input);
    ~SubsetGenerator() = default;
    SubsetGenerator(const SubsetGenerator&) = delete;
    SubsetGenerator(SubsetGenerator&&) = delete;
    SubsetGenerator& operator=(const SubsetGenerator&) = delete;
    SubsetGenerator& operator=(SubsetGenerator&&) = delete;

    /*!
     * \brief HasNext returns a value indicating whether a next item exists.
     * \return A value indicating whether a next item exists.
     */
    bool HasNext() const;

    /*!
     * \brief Gets the next substring.
     * \return The next substring.
     */
    std::wstring GetNext();
};

#endif // SUBSETGENERATOR_H
