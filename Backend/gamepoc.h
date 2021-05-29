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
#include <memory>
#include "parser.h"
#include "dot.h"
#include "dotgenerator.h"
#include "randomdotgenerator.h"

namespace Backend {

    /*!
     * \class GamePoc
     * \brief The GamePoc class represents a Proof-of-Concept regarding the evaluation
     *        of functions into graph data.
     *
     * Because it is a POC, the documentation is sparse.
     */
    class GamePoc final
    {
    private:
        std::vector<std::shared_ptr<Dot>> dots;
        std::vector<std::string> funcStrings;
        std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>> graphs;

        Parser parser;
        std::shared_ptr<DotGenerator> dotGenerator;

    public:
        /*!
         * \brief Initializes a new instance using the supplied dot generator.
         * \param dotGenerator The generator to use in creation of a new game.
         */
        GamePoc(std::shared_ptr<DotGenerator> dotGenerator = std::make_shared<RandomDotGenerator>(8, 2));
        ~GamePoc() = default;
        GamePoc(const GamePoc&) = delete;
        GamePoc(GamePoc&&) = delete;
        GamePoc& operator=(const GamePoc&) = delete;
        GamePoc& operator=(GamePoc&&) = delete;

        /*!
         * \brief Simulates the evaluation of the functions supplied by the user.
         * \param funcStrings The user-supplied string representations of functions.
         */
        void Update(const std::vector<std::string> & funcStrings);

        /*!
         * \brief Gets the sorted data calculated in the update representing the graphs of the functions.
         * \return The graph data in a graph.branch.(xy).data-coordinate hierarchy.
         */
        const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GetGraphs() const;

        /*!
         * \brief Gets the dot information.
         * \return The dot information.
         */
        const std::vector<std::shared_ptr<Dot>>& GetDots() const;

        /*!
         * \brief Clears all input of the game.
         */
        void Clear();

    private:
        void CreateItems();
        void CreateGraphs();
        void PushEmptyGraph();
        void CreateDots();
        void CheckDots(std::shared_ptr<Expression> expression, std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData);
        void ResetDots();
    };

}

#endif // GAMEPOC_H
