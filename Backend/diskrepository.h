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

#ifndef DISKREPOSITORY_H
#define DISKREPOSITORY_H

#include "repository.h"

namespace Backend
{
    /*!
     * \class DiskRepository
     * \brief The DiskRepository class persists a game to disk.
     */
    class DiskRepository final : public Repository
    {
    public:
        /*!
         * \brief Initializes a new instance.
         */
        DiskRepository();

        /*!
         * \reimp
         */
        virtual std::pair<bool, std::wstring> Save(const Game& game, const std::wstring& identifier);

        /*!
         * \reimp
         */
        virtual std::pair<bool, std::wstring> Load(const std::wstring& identifier, Game& game);
    };
}

#endif // DISKREPOSITORY_H
