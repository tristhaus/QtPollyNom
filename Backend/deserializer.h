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

#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <iostream>
#include "classes.h"
#include "rapidjson/document.h"

namespace Backend
{
    /*!
     * \brief The DeSerializer class provides de/serialization of \ref Game.
     */
    class DeSerializer final
    {
    private:
        constexpr static const wchar_t * const KeyDataVersion = L"dataVersion";
        constexpr static const wchar_t * const KeyX = L"x";
        constexpr static const wchar_t * const KeyY = L"y";
        constexpr static const wchar_t * const KeyRadius = L"radius";
        constexpr static const wchar_t * const KeyKind = L"kind";
        constexpr static const wchar_t * const ValueKindGood = L"good";
        constexpr static const wchar_t * const ValueKindBad = L"bad";
        constexpr static const wchar_t * const KeyDots = L"dots";
        constexpr static const wchar_t * const KeyFunctions = L"functions";

    public:
        /*!
         * \brief Initializes a new instance.
         */
        DeSerializer();
        DeSerializer(const DeSerializer&) = default;
        DeSerializer(DeSerializer&&) = delete;
        DeSerializer& operator=(const DeSerializer&) = delete;
        DeSerializer& operator=(DeSerializer&&) = delete;

        /*!
         * \brief Serializes the provided game to the provided stream in a JSON representation.
         * \param game The game to serialize.
         * \param os The stream that shall accept the serialized game.
         */
        void Serialize(const Game & game, std::wostream & wos);

        /*!
         * \brief Deserializes a game from the provided stream (in a JSON representation) into the provided game.
         * \param is The stream from which the game is to be deserialized.
         * \param game The game that shall accept the deserialized information.
         * \return true and empty string if successful, false and error message if not.
         */
        std::pair<bool, std::wstring> Deserialize(std::wistream & wis, Game & game);

    private:
        void TransformGameDotIntoSerializationDot(std::shared_ptr<Dot>& gameDot, rapidjson::GenericValue<rapidjson::UTF16<wchar_t>>& serializationDot, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
        void AddSerializationTime(rapidjson::GenericDocument<rapidjson::UTF16<wchar_t>>& document, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
        std::wstring GetKindAsString(const bool isGood);
        bool TryParseKindFromString(const std::wstring string, bool& kind);
    };

}

#endif // DESERIALIZER_H
