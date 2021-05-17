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

#ifndef GAMEPOC_H
#define GAMEPOC_H

#include <vector>
#include "dot.h"

class GamePoc
{
    std::vector<Dot> dots;
    std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>> graphs;

public:
    GamePoc();
    ~GamePoc() = default;
    GamePoc(const GamePoc&) = delete;
    GamePoc(GamePoc&&) = delete;
    GamePoc& operator=(const GamePoc&) = delete;
    GamePoc& operator=(GamePoc&&) = delete;

    void Update();
    const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GetGraphs() const;
    std::vector<Dot>& GetDots();

private:
    void SetupPOCItems();
    void SetupPOCGraphs();
    void SetupPOCDots();
};

#endif // GAMEPOC_H
