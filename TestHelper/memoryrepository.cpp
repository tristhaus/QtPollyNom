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

#include "memoryrepository.h"
#include "../Backend/deserializer.h"
#include <sstream>

MemoryRepository::MemoryRepository()
 : Repository(DeSerializer())
{
}

std::pair<bool, std::wstring> MemoryRepository::Save(const Game& game, const std::wstring& identifier)
{
    std::wstringstream wss;
    deserializer.Serialize(game, wss);

    std::wstring content;
    wss >> content;

    storage[identifier] = content;

    return std::make_pair<bool, std::wstring>(true, L"");
}

std::pair<bool, std::wstring> MemoryRepository::Load(const std::wstring& identifier, Game& game)
{
    std::wstring content;
    bool found = TryGetByIdentifier(identifier, content);

    if(!found)
    {
        return std::make_pair<bool, std::wstring>(false, L"content not found by identifier");
    }

    std::wstringstream wss;
    wss << content;
    wss.seekg(0, std::ios::beg);

    return deserializer.Deserialize(wss, game);
}

bool MemoryRepository::TryGetByIdentifier(const std::wstring& identifier, std::wstring& string)
{
    auto it = storage.find(identifier);

    if(it == storage.end())
    {
        return false;
    }

    string = it->second;
    return true;
}
