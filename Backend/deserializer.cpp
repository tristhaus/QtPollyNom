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
#include <cstring>
#include "deserializer.h"
#include "game.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/istreamwrapper.h"

#undef __STDC_WANT_LIB_EXT1__

namespace Backend
{
    DeSerializer::DeSerializer()
    {
    }

    void DeSerializer::AddSerializationTime(rapidjson::GenericDocument<rapidjson::UTF16<wchar_t>> & document, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> & allocator)
    {
        rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> key, value;

        time_t now;
        time(&now);
        tm tm;
        wchar_t buf[sizeof "2011-10-08T07:07:09Z"];
        gmtime_s(&tm, &now);
        wcsftime(buf, sizeof buf, L"%FT%TZ", &tm);

        std::wstring result(buf);

        key.SetString(L"creationDate");
        value.SetString(result.c_str(), static_cast<rapidjson::SizeType>(result.length()), allocator);

        document.AddMember(key, value, allocator);
    }

    void DeSerializer::TransformGameDotIntoSerializationDot(std::shared_ptr<Dot> & gameDot, rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> & serializationDot, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> & allocator)
    {
        rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> key, value;
        serializationDot.SetObject();

        key.SetString(KeyX, static_cast<rapidjson::SizeType>(wcslen(KeyX)), allocator);
        value.SetDouble(gameDot->GetCoordinates().first);
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyY, static_cast<rapidjson::SizeType>(wcslen(KeyY)), allocator);
        value.SetDouble(gameDot->GetCoordinates().second);
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyRadius, static_cast<rapidjson::SizeType>(wcslen(KeyRadius)), allocator);
        value.SetDouble(gameDot->GetRadius());
        serializationDot.AddMember(key, value, allocator);

        key.SetString(KeyKind, static_cast<rapidjson::SizeType>(wcslen(KeyKind)), allocator);
        auto gameDotKind = GetKindAsString(gameDot->IsGood());
        value.SetString(gameDotKind.c_str(), static_cast<rapidjson::SizeType>(gameDotKind.length()), allocator);
        serializationDot.AddMember(key, value, allocator);
    }

    std::wstring DeSerializer::GetKindAsString(const bool isGood)
    {
        return isGood ? ValueKindGood : ValueKindBad;
    }

    bool DeSerializer::TryParseKindFromString(const std::wstring string, bool & kind)
    {
        if(string.compare(ValueKindGood) == 0)
        {
            kind = true;
            return true;
        }
        else if(string.compare(ValueKindBad) == 0)
        {
            kind = false;
            return true;
        }

        return false;
    }

    void DeSerializer::Serialize(const Game & game, std::wostream & wos)
    {
        rapidjson::GenericDocument<rapidjson::UTF16<wchar_t>> document;
        document.SetObject();
        auto & allocator = document.GetAllocator();

        rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> key, value;

        key.SetString(KeyDataVersion, static_cast<rapidjson::SizeType>(wcslen(KeyDataVersion)), allocator);
        value.SetString(L"1");

        document.AddMember(key, value, allocator);

        AddSerializationTime(document, allocator);

        rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> dots;
        dots.SetArray();

        auto gameDots = game.GetDots();
        auto gameDotsIt = gameDots.begin();
        auto gameDotsEnd = gameDots.end();
        for(; gameDotsIt != gameDotsEnd; ++gameDotsIt)
        {
            rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> dot;

            TransformGameDotIntoSerializationDot((*gameDotsIt), dot, allocator);

            dots.PushBack(dot, allocator);
        }

        key.SetString(KeyDots, static_cast<rapidjson::SizeType>(wcslen(KeyDots)), allocator);
        document.AddMember(key, dots, allocator);

        rapidjson::GenericValue<rapidjson::UTF16<wchar_t>> functions;
        functions.SetArray();

        auto gameFunctions = game.GetFunctions();
        auto gameFunctionsIt = gameFunctions.begin();
        auto gameFunctionsEnd = gameFunctions.end();
        for(; gameFunctionsIt != gameFunctionsEnd; ++gameFunctionsIt)
        {
            value.SetString(gameFunctionsIt->c_str(), static_cast<rapidjson::SizeType>(gameFunctionsIt->length()), allocator);
            functions.PushBack(value, allocator);
        }

        key.SetString(KeyFunctions, static_cast<rapidjson::SizeType>(wcslen(KeyFunctions)), allocator);
        document.AddMember(key, functions, allocator);

        rapidjson::WOStreamWrapper osw(wos);

        rapidjson::Writer<rapidjson::WOStreamWrapper, rapidjson::UTF16<wchar_t>, rapidjson::ASCII<char>> writer(osw);
        document.Accept(writer);
    }

    std::pair<bool, std::wstring> MakeError(std::wstring error)
    {
        return std::make_pair<bool, std::wstring>(false, error.c_str());
    }

    std::pair<bool, std::wstring> DeSerializer::Deserialize(std::wistream& wis, Game& game)
    {
        rapidjson::WIStreamWrapper wisw(wis);

        rapidjson::GenericDocument<rapidjson::UTF16<wchar_t>> document;
        document.ParseStream(wisw);

        if(!(document.IsObject()))
        {
            return MakeError(L"did not parse to object");
        }

        if(!(document.HasMember(KeyDataVersion) && document[KeyDataVersion].IsString() && std::wcscmp(document[KeyDataVersion].GetString(), L"1") == 0))
        {
            return MakeError(L"no valid data version found");
        }

        if(!(document.HasMember(KeyDots) && document[KeyDots].IsArray()))
        {
            return MakeError(L"no valid dots member found");
        }

        auto & dots = document[KeyDots];
        auto dotsIt = dots.Begin();
        auto dotsEnd = dots.End();

        std::vector<std::shared_ptr<Dot>> deserializedDots;

        for(; dotsIt != dotsEnd; ++dotsIt)
        {
            if(!(dotsIt->HasMember(KeyX) && (*dotsIt)[KeyX].IsDouble()
                 && dotsIt->HasMember(KeyY) && (*dotsIt)[KeyY].IsDouble()
                 && dotsIt->HasMember(KeyKind) && (*dotsIt)[KeyKind].IsString()
                 && dotsIt->HasMember(KeyRadius) && (*dotsIt)[KeyRadius].IsDouble()))
            {
                return MakeError(L"invalid dot found");
            }

            auto deserializedX = (*dotsIt)[KeyX].GetDouble();
            auto deserializedY = (*dotsIt)[KeyY].GetDouble();
            auto deserializedRadius = (*dotsIt)[KeyRadius].GetDouble();

            if(deserializedRadius < 0.0)
            {
                return MakeError(L"radius of dot must be positive");
            }

            bool deserializedKind;
            if(!TryParseKindFromString((*dotsIt)[KeyKind].GetString(), deserializedKind))
            {
                return MakeError(L"invalid kind in dot found");
            }

            auto deserializedDot = std::make_shared<Dot>(deserializedX, deserializedY, deserializedKind, deserializedRadius);

            deserializedDots.push_back(deserializedDot);
        }

        if(!(document.HasMember(KeyFunctions) && document[KeyFunctions].IsArray()))
        {
            return MakeError(L"no valid functions member found");
        }

        auto & functions = document[KeyFunctions];
        auto functionsIt = functions.Begin();
        auto functionsEnd = functions.End();

        std::vector<std::wstring> deserializedFunctions;

        for(; functionsIt != functionsEnd; ++functionsIt)
        {
            if(!functionsIt->IsString())
            {
                return MakeError(L"function is not a string");
            }

            auto deserializedFunction = std::wstring(functionsIt->GetString());

            deserializedFunctions.push_back(deserializedFunction);
        }

        while(deserializedFunctions.size() > 5)
        {
            deserializedFunctions.pop_back();
        }

        while(deserializedFunctions.size() < 5)
        {
            deserializedFunctions.push_back(L"");
        }

        game.Clear();
        game.SetDots(deserializedDots);
        game.Update(deserializedFunctions);

        return std::make_pair<bool, std::wstring>(true, L"");
    }

}
