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

#include "gamepoc.h"
#include "evaluator.h"
#include "randomdotgenerator.h"

namespace Backend {

    //GamePoc::GamePoc() : GamePoc()
    //{
    //}

    GamePoc::GamePoc(std::shared_ptr<DotGenerator> dotGenerator)
        : dotGenerator(dotGenerator)
    {
        this->CreateItems();
    }

    void GamePoc::Update(const std::vector<std::string> & funcStrings)
    {
        this->funcStrings = funcStrings;
        this->CreateGraphs();
    }

    const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GamePoc::GetGraphs() const
    {
        return graphs;
    }

    const std::vector<std::shared_ptr<Dot>>& GamePoc::GetDots() const
    {
        return dots;
    }

    void GamePoc::Clear()
    {
        this->funcStrings.clear();
        this->graphs.clear();
        this->ResetDots();
    }

    void GamePoc::CreateItems()
    {
        this->CreateGraphs();
        this->CreateDots();
    }

    void GamePoc::CreateGraphs()
    {
        this->graphs.clear();
        this->ResetDots();

        for(unsigned long int i=0; i < funcStrings.size() && i < 5; ++i)
        {
            if(funcStrings[i].empty())
            {
                this->PushEmptyGraph();
                continue;
            }

#ifdef _DEBUG
            if(funcStrings[i] == "slow")
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                continue;
            }
#endif

            auto expression = parser.Parse(funcStrings[i]);
            if(!expression)
            {
                this->PushEmptyGraph();
                continue;
            }

            Evaluator evaluator(expression, -10.5, 10.5, 1000.0);
            auto graph = evaluator.Evaluate();
            this->graphs.push_back(graph);
            this->CheckDots(expression, graph);
        }
    }

    void GamePoc::PushEmptyGraph()
    {
        this->graphs.push_back(std::vector<std::pair<std::vector<double>, std::vector<double>>>());
    }

    void GamePoc::CreateDots()
    {
        this->dots = this->dotGenerator->Generate();
    }

    void GamePoc::CheckDots(std::shared_ptr<Expression> expression, std::vector<std::pair<std::vector<double>, std::vector<double>>> graphData)
    {
        for(auto dotIterator = this->dots.begin(); dotIterator != this->dots.end(); ++dotIterator)
        {
            (*dotIterator)->CheckForHit(expression, graphData);
        }
    }

    void GamePoc::ResetDots()
    {
        for(auto dotIterator = this->dots.begin(); dotIterator != this->dots.end(); ++dotIterator)
        {
            (*dotIterator)->ResetIsActive();
        }
    }

}
