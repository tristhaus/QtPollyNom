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

#include <chrono>
#include <thread>
#include <algorithm>

#include "game.h"
#include "evaluator.h"
#include "randomdotgenerator.h"

namespace Backend {

    void Backend::Game::Init()
    {
        this->CreateItems();
        this->Clear();
    }

    Game::Game(std::shared_ptr<DotGenerator> dotGenerator)
        : dotGenerator(dotGenerator)
    {
        this->Init();
    }

    void Game::Remake()
    {
        this->Init();
    }

    void Game::Update(const std::vector<std::string> & funcStrings)
    {
        this->updateFuncStrings = funcStrings;
        this->CreateGraphs();
    }

    bool Game::IsParseable(const std::string& input) const
    {
        return this->parser.IsParseable(input);
    }

    const std::vector<std::string> Game::GetFunctions() const
    {
        return this->updateFuncStrings;
    }

    const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& Game::GetGraphs() const
    {
        return graphs;
    }

    const std::vector<std::shared_ptr<Dot>>& Game::GetDots() const
    {
        return dots;
    }

    int Game::GetScore() const
    {
        if(std::any_of(this->dots.begin(), this->dots.end(), [](auto dot){ return dot->IsActive() && !dot->IsGood(); }))
        {
            return -1;
        }

        int score(0);

        std::vector<std::set<unsigned long int>> copy(this->dotHitBy);

        while(std::any_of(copy.begin(), copy.end(), [&](std::set<unsigned long int> set){ return !set.empty(); }))
        {
            std::vector<int> counts;

            std::for_each(copy.begin(), copy.end(), [&](std::set<unsigned long int> set)
            {
                std::for_each(set.begin(), set.end(), [&](unsigned long int index)
                {
                    while(counts.size() <= index)
                    {
                        counts.push_back(0);
                    }

                    ++counts[index];
                });
            });

            auto index = static_cast<unsigned long int>(distance(counts.begin(), std::max_element(counts.begin(), counts.end())));

            score += static_cast<int>(std::pow(2, counts[index])) - 1;

            std::for_each(copy.begin(), copy.end(), [&](std::set<unsigned long int> & set)
            {
                if(set.find(index) != set.end())
                {
                    set.clear();
                }
            });
        }

        return score;
    }

    void Game::Clear()
    {
        this->updateFuncStrings.clear();
        this->funcStringsEvaluated.clear();
        this->graphs.clear();
        this->ResetDots();
    }

    void Game::CreateItems()
    {
        this->CreateGraphs();
        this->CreateDots();
    }

    void Game::CreateGraphs()
    {
        this->ResetDots();

        for(unsigned long int i=0; i < updateFuncStrings.size() && i < 5; ++i)
        {
            if(updateFuncStrings[i].empty())
            {
                this->PutEmptyGraphAtIndex(i);
                this->SaveFunctionAtIndex(i, updateFuncStrings[i]);
                continue;
            }

#ifdef _DEBUG
            if(updateFuncStrings[i] == "slow")
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                continue;
            }
#endif

            auto expression = parser.Parse(updateFuncStrings[i]);
            if(!expression)
            {
                this->PutEmptyGraphAtIndex(i);
                continue;
            }

            if(funcStringsEvaluated.size() <= i || funcStringsEvaluated[i] != updateFuncStrings[i])
            {
                Evaluator evaluator(expression, -10.5, 10.5, 1000.0);
                auto graph = evaluator.Evaluate();
                this->PutGraphAtIndex(i, graph);
                this->SaveFunctionAtIndex(i, updateFuncStrings[i]);
            }

            this->CheckDots(i, expression, this->graphs[i]);
        }
    }

    void Game::PutEmptyGraphAtIndex(unsigned long int index)
    {
        this->PutGraphAtIndex(index, std::vector<std::pair<std::vector<double>, std::vector<double>>>());
    }

    void Game::PutGraphAtIndex(unsigned long int index, std::vector<std::pair<std::vector<double>, std::vector<double>>> graph)
    {
        while(this->graphs.size() < index + 1)
        {
            this->graphs.push_back(std::vector<std::pair<std::vector<double>, std::vector<double>>>());
        }

        this->graphs[index] = graph;
    }

    void Game::SaveFunctionAtIndex(unsigned long int index, std::string funcString)
    {
        while(this->funcStringsEvaluated.size() < index + 1)
        {
            this->funcStringsEvaluated.emplace_back("");
        }

        this->funcStringsEvaluated[index] = funcString;
    }

    void Game::CreateDots()
    {
        this->dots = this->dotGenerator->Generate();
        this->dotHitBy = std::vector<std::set<unsigned long int>>(this->dots.size());
    }

    void Game::CheckDots(unsigned long int graphIndex, std::shared_ptr<Expression> expression, std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData)
    {
        const auto dotCount = this->dots.size();
        for(size_t dotIndex = 0; dotIndex < dotCount; ++dotIndex)
        {
            auto & dot = this->dots[dotIndex];

            bool wasHit = dot->CheckForHit(expression, graphData);
            if(wasHit)
            {
                dotHitBy[dotIndex].insert(graphIndex);
            }
        }
    }

    void Game::ResetDots()
    {
        for(auto dotIterator = this->dots.begin(); dotIterator != this->dots.end(); ++dotIterator)
        {
            (*dotIterator)->ResetIsActive();
        }

        std::for_each(this->dotHitBy.begin(), this->dotHitBy.end(), [](std::set<unsigned long int> &set){ set.clear(); });
    }

}
