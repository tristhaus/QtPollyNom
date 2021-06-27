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

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "parser.h"
#include "dot.h"
#include "dotgenerator.h"
#include "randomdotgenerator.h"

namespace Backend {

    /*!
     * \class Game
     * \brief The Game class represents an active game.
     *        It evaluates functions into graph data and handles dots.
     */
    class Game final
    {
    private:
        std::vector<std::shared_ptr<Dot>> dots;
        std::vector<std::string> updateFuncStrings;
        std::vector<std::string> funcStringsEvaluated;
        std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>> graphs;

        Parser parser;
        std::shared_ptr<DotGenerator> dotGenerator;

    public:
        /*!
         * \brief Initializes a new instance using the supplied dot generator.
         * \param dotGenerator The generator to use in creation of a new game.
         */
        Game(std::shared_ptr<DotGenerator> dotGenerator = std::make_shared<RandomDotGenerator>(8, 2));
        ~Game() = default;
        Game(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(const Game&) = delete;
        Game& operator=(Game&&) = delete;

        /*!
         * \brief Evaluates the functions supplied by the user.
         * \param funcStrings The user-supplied string representations of functions.
         */
        void Update(const std::vector<std::string> & funcStrings);

        /*!
         * \brief IsParseable indicates whether the supplied string can parse to an expression.
         * \param input The string to check for parseability.
         * \return true if the argument is parseable, false otherwise.
         */
        bool IsParseable(const std::string & input) const;

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
        void PutEmptyGraphAtIndex(unsigned long int index);
        void PutGraphAtIndex(unsigned long index, std::vector<std::pair<std::vector<double>, std::vector<double> > > graph);
        void SaveFunctionAtIndex(unsigned long index, std::string funcString);
        void CreateDots();
        void CheckDots(std::shared_ptr<Expression> expression, std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData);
        void ResetDots();
    };

}

#endif // GAME_H
