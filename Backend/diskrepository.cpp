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

#include "diskrepository.h"
#include <fstream>
#include <filesystem>

namespace Backend
{
    DiskRepository::DiskRepository()
        : Repository(DeSerializer())
    {
    }

    std::pair<bool, std::wstring> Backend::DiskRepository::Save(const Game& game, const std::wstring& identifier)
    {
        std::wofstream wofs(identifier);

        if(!(wofs.is_open() && wofs.good()))
        {
            return std::make_pair<bool, std::wstring>(false, std::wstring(L"unable to open stream for writing: ") + identifier);
        }

        deserializer.Serialize(game, wofs);

        if(!(wofs.is_open() && wofs.good()))
        {
            return std::make_pair<bool, std::wstring>(false, std::wstring(L"bad stream after writing to: ") + identifier);
        }

        wofs.close();

        return std::make_pair<bool, std::wstring>(true, L"");
    }

    std::pair<bool, std::wstring> Backend::DiskRepository::Load(const std::wstring& identifier, Game& game)
    {
        try
        {
            std::filesystem::path path(identifier);
            if(!std::filesystem::exists(path))
            {
                return std::make_pair<bool, std::wstring>(false, std::wstring(L"file does not exist: ") + identifier);
            }
        }
        catch (const std::exception&)
        {
            return std::make_pair<bool, std::wstring>(false, std::wstring(L"exception when opening: ") + identifier);
        }

        std::wifstream wifs(identifier);

        if(!(wifs.is_open() && wifs.good()))
        {
            return std::make_pair<bool, std::wstring>(false, std::wstring(L"unable to open stream for reading: ") + identifier);
        }

        auto deserializationResult = deserializer.Deserialize(wifs, game);

        wifs.close();

        return deserializationResult;
    }
}
