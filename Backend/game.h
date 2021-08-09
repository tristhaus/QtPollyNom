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
#include <set>
#include <memory>
#include "classes.h"
#include "parser.h"
#include "dot.h"
#include "dotgenerator.h"
#include "randomdotgenerator.h"
#include "repository.h"
#include "diskrepository.h"

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
        std::vector<std::wstring> updateFuncStrings;
        std::vector<std::wstring> funcStringsEvaluated;
        std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>> graphs;

        Parser parser;
        std::shared_ptr<DotGenerator> dotGenerator;
        std::shared_ptr<Repository> repository;
        std::vector<std::set<unsigned long int>> dotHitBy;

    public:
        /*!
         * \brief Initializes a new instance using the supplied dot generator.
         * \param dotGenerator The generator to use in creation of a new game.
         * \param repository The repository used to persist games.
         */
        Game(std::shared_ptr<DotGenerator> dotGenerator = std::make_shared<RandomDotGenerator>(8, 2),
             std::shared_ptr<Repository> repository = std::make_shared<DiskRepository>());
        ~Game() = default;
        Game(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(const Game&) = delete;
        Game& operator=(Game&&) = default;

        /*!
         * \brief Remake Resets the game and creates new dots from the dot generator.
         */
        void Remake();

        /*!
         * \brief Evaluates the functions supplied by the user.
         * \param funcStrings The user-supplied string representations of functions.
         */
        void Update(const std::vector<std::wstring> & funcStrings);

        /*!
         * \brief CreateGraphs Creates graphs from the contained functions.
         */
        void CreateGraphs();

        /*!
         * \brief IsParseable indicates whether the supplied string can parse to an expression.
         * \param input The string to check for parseability.
         * \return true if the argument is parseable, false otherwise.
         */
        bool IsParseable(const std::wstring & input) const;

        /*!
         * \brief Gets the functions contained.
         * \return The functions.
         */
        const std::vector<std::wstring> GetFunctions() const;

        /*!
         * \brief Gets the sorted data calculated in the update representing the graphs of the functions.
         * \return The graph data in a graph.branch.(xy).data-coordinate hierarchy.
         */
        const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GetGraphs() const;

        /*!
         * \brief Sets the dot information.
         * \param newDots The dots to set.
         */
        void SetDots(std::vector<std::shared_ptr<Dot>> newDots);

        /*!
         * \brief Gets the dot information.
         * \return The dot information.
         */
        const std::vector<std::shared_ptr<Dot>>& GetDots() const;

        /*!
         * \brief Gets the score from the dot hitting.
         * \return The score, negative if a bad dot was hit.
         */
        int GetScore() const;

        /*!
         * \brief Clears all input of the game.
         */
        void Clear();

        /*!
         * \brief Save the game using the repository.
         * \param identifier The identifier under which to save.
         * \return A pair of bool (indicating success) and string (describing the error if unsuccessful).
         */
        std::pair<bool, std::wstring> Save(std::wstring identifier) const;

        /*!
         * \brief Load the game from the repository, changing the current instance.
         * \param identifier The identifier from which to load.
         * \return A pair of bool (indicating success) and string (describing the error if unsuccessful).
         */
        std::pair<bool, std::wstring> Load(std::wstring identifier);

    private:
        void Init();
        void CreateItems();
        void PutEmptyGraphAtIndex(unsigned long int index);
        void PutGraphAtIndex(unsigned long index, std::vector<std::pair<std::vector<double>, std::vector<double> > > graph);
        void SaveFunctionAtIndex(unsigned long index, std::wstring funcString);
        void CreateDots();
        void CheckDots(unsigned long int graphIndex, std::shared_ptr<Expression> expression, std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData);
        void ResetDots();
    };

}

#endif // GAME_H
