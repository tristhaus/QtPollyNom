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

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include "classes.h"

namespace Backend
{
    /*!
     * \brief The Repository class forms the base for ways to persist games.
     *
     * It is abstract.
     */
    class Repository
    {
    public:
        explicit Repository()
        {
        }
        virtual ~Repository()
        {
        }
        Repository(const Repository&) = delete;
        Repository(Repository&&) = delete;
        Repository& operator=(const Repository&) = delete;
        Repository& operator=(Repository&&) = delete;

        /*!
         * \brief Saves a game to the storage.
         * \param game The game to save.
         * \param identifier The identifier, which may be a file path if the underlying storage is the disk.
         * \return A pair of bool (indicating success) and string (describing the error if unsuccessful).
         */
        virtual std::pair<bool, std::wstring> Save(const Game & game, const std::wstring & identifier) = 0;

        /*!
         * \brief Loads a game from storage into the provided game instance.
         * \param identifier The identifier, which may be a file path if the underlying storage is the disk.
         * \param game The game which shall hold the loaded information.
         * \return A pair of bool (indicating success) and string (describing the error if unsuccessful).
         */
        virtual std::pair<bool, std::wstring> Load(const std::wstring & identifier, Game & game) = 0;
    };
}

#endif // REPOSITORY_H
