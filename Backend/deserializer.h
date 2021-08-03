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
        constexpr static const char * const KeyDataVersion = u8"dataVersion";
        constexpr static const char * const KeyX = u8"x";
        constexpr static const char * const KeyY = u8"y";
        constexpr static const char * const KeyRadius = u8"radius";
        constexpr static const char * const KeyKind = u8"kind";
        constexpr static const char * const ValueKindGood = u8"good";
        constexpr static const char * const ValueKindBad = u8"bad";
        constexpr static const char * const KeyDots = u8"dots";
        constexpr static const char * const KeyFunctions = u8"functions";

    public:
        /*!
         * \brief Initializes a new instance.
         */
        DeSerializer();
        DeSerializer(const DeSerializer&) = delete;
        DeSerializer(DeSerializer&&) = delete;
        DeSerializer& operator=(const DeSerializer&) = delete;
        DeSerializer& operator=(DeSerializer&&) = delete;

        /*!
         * \brief Serializes the provided game to the provided stream in a JSON representation.
         * \param game The game to serialize.
         * \param os The stream that shall accept the serialized game.
         */
        void Serialize(const Game & game, std::ostream & os);

        /*!
         * \brief Deserializes a game from the provided stream (in a JSON representation) into the provided game.
         * \param is The stream from which the game is to be deserialized.
         * \param game The game that shall accept the deserialized information.
         * \return true and empty string if successful, false and error message if not.
         */
        std::pair<bool, std::string> Deserialize(std::istream & is, Game & game);

    private:
        void TransformGameDotIntoSerializationDot(std::shared_ptr<Dot>& gameDot, rapidjson::Value& serializationDot, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
        void AddSerializationTime(rapidjson::Document& document, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
        std::string GetKindAsString(const bool isGood);
        bool TryParseKindFromString(const std::string string, bool& kind);
    };

}

#endif // DESERIALIZER_H
