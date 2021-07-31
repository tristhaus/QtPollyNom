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

#define __STDC_WANT_LIB_EXT1__ 1

#include <ctime>
#include "deserializer.h"
#include "game.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

#undef __STDC_WANT_LIB_EXT1__

namespace Backend
{
    DeSerializer::DeSerializer()
    {
    }

    std::string GetKindAsString(bool isGood)
    {
        return isGood ? "good" : "bad";
    }

    void DeSerializer::AddSerializationTime(rapidjson::Document & document, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> & allocator)
    {
        rapidjson::Value key;
        rapidjson::Value value;

        time_t now;
        time(&now);
        tm tm;
        char buf[sizeof "2011-10-08T07:07:09Z"];
        gmtime_s(&tm, &now);
        strftime(buf, sizeof buf, "%FT%TZ", &tm);

        key.SetString("creationDate");
        value.SetString(buf, sizeof buf, allocator);

        document.AddMember(key, value, allocator);
    }

    void DeSerializer::TransformGameDotIntoSerializationDot(std::shared_ptr<Dot> & gameDot, rapidjson::Value & serializationDot, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> & allocator)
    {
        rapidjson::Value key, value;
        serializationDot.SetObject();

        key.SetString(KeyX, static_cast<rapidjson::SizeType>(strlen(KeyX)), allocator);
        value.SetDouble(gameDot->GetCoordinates().first);
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyY, static_cast<rapidjson::SizeType>(strlen(KeyY)), allocator);
        value.SetDouble(gameDot->GetCoordinates().second);
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyRadius, static_cast<rapidjson::SizeType>(strlen(KeyRadius)), allocator);
        value.SetDouble(gameDot->GetRadius());
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyKind, static_cast<rapidjson::SizeType>(strlen(KeyKind)), allocator);
        std::string gameDotKind = GetKindAsString(gameDot->IsGood());
        value.SetString(gameDotKind.c_str(), static_cast<rapidjson::SizeType>(gameDotKind.length()), allocator);
        serializationDot.AddMember(key, value, allocator);
    }

    void DeSerializer::Serialize(const Game & game, std::ostream & os)
    {
        rapidjson::Document document;
        document.SetObject();
        auto & allocator = document.GetAllocator();

        rapidjson::Value key;
        rapidjson::Value value;

        key.SetString(KeyDataVersion, static_cast<rapidjson::SizeType>(strlen(KeyDataVersion)), allocator);
        value.SetString("1");

        document.AddMember(key, value, allocator);

        AddSerializationTime(document, allocator);

        rapidjson::Value dots;
        dots.SetArray();

        auto gameDots = game.GetDots();
        auto gameDotsIt = gameDots.begin();
        auto gameDotsEnd = gameDots.end();
        for(; gameDotsIt != gameDotsEnd; ++gameDotsIt)
        {
            rapidjson::Value dot;

            TransformGameDotIntoSerializationDot((*gameDotsIt), dot, allocator);

            dots.PushBack(dot, allocator);
        }

        key.SetString(KeyDots, static_cast<rapidjson::SizeType>(strlen(KeyDots)), allocator);
        document.AddMember(key, dots, allocator);

        rapidjson::Value functions;
        functions.SetArray();

        auto gameFunctions = game.GetFunctions();
        auto gameFunctionsIt = gameFunctions.begin();
        auto gameFunctionsEnd = gameFunctions.end();
        for(; gameFunctionsIt != gameFunctionsEnd; ++gameFunctionsIt)
        {
            value.SetString(gameFunctionsIt->c_str(), static_cast<rapidjson::SizeType>(gameFunctionsIt->length()), allocator);
            functions.PushBack(value, allocator);
        }

        key.SetString(KeyFunctions, static_cast<rapidjson::SizeType>(strlen(KeyFunctions)), allocator);
        document.AddMember(key, functions, allocator);

        rapidjson::OStreamWrapper osw(os);

        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        document.Accept(writer);
    }

}
