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

#ifndef MEMORYREPOSITORY_H
#define MEMORYREPOSITORY_H

#include "../Backend/repository.h"
#include <map>

using namespace Backend;

/*!
 * \class MemoryRepository
 * \brief The MemoryRepository class persists games to memory.
 */
class MemoryRepository final : public Repository
{
private:
    std::map<std::wstring, std::wstring> storage;

public:
    /*!
     * \brief Initializes a new instance.
     */
    MemoryRepository();

    /*!
     * \reimp
     */
    virtual std::pair<bool, std::wstring> Save(const Game& game, const std::wstring& identifier);

    /*!
     * \reimp
     */
    virtual std::pair<bool, std::wstring> Load(const std::wstring& identifier, Game& game);

    /*!
     * \brief TryGetByIdentifier Attempts to get the string stored under the identifier.
     * \param identifier The identifier to try and get the string for.
     * \param string The stored string, if any.
     * \return true if found.
     */
    bool TryGetByIdentifier(const std::wstring & identifier, std::wstring & string);
};

#endif // MEMORYREPOSITORY_H
