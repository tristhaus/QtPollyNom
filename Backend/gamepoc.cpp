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

#include "gamepoc.h"
#include "parser.h"
#include "evaluator.h"

namespace Backend {

    GamePoc::GamePoc()
    {
        this->SetupPOCItems();
    }

    void GamePoc::Update()
    {
        this->SetupPOCGraphs();
    }

    const std::vector<std::vector<std::pair<std::vector<double>, std::vector<double>>>>& GamePoc::GetGraphs() const
    {
        return graphs;
    }

    const std::vector<std::shared_ptr<Dot>>& GamePoc::GetDots() const
    {
        return dots;
    }

    void GamePoc::SetupPOCItems()
    {
        SetupPOCGraphs();
        SetupPOCDots();
    }

    void GamePoc::SetupPOCGraphs()
    {
        this->graphs.clear();

        Parser parser;

        {
            auto hyperbolaExpression = parser.Parse("1.0/x");

            // note that unconditionally passing all dots is wrong
            // eventually, only those that have not been hit yet shall be passed
            Evaluator hyperbolaEvaluator(hyperbolaExpression, dots, -10.5, 10.5, 1000.0);
            auto hyperbolaGraph = hyperbolaEvaluator.Evaluate();
            this->graphs.push_back(hyperbolaGraph);
        }
        {
            auto polynomialExpression = parser.Parse("(x-3.0)*(x+4.0)");
            Evaluator polynomialEvaluator(polynomialExpression, dots, -10.5, 10.5, 1000.0);
            auto polynomialGraph = polynomialEvaluator.Evaluate();
            this->graphs.push_back(polynomialGraph);
        }
    }

    void GamePoc::SetupPOCDots()
    {
        // dots to be made active
        dots.push_back(std::make_shared<Dot>(1.0, 1.0, true));
        dots.push_back(std::make_shared<Dot>(-8.0, -0.25, true));

        // dots to remain inactive
        dots.push_back(std::make_shared<Dot>(5.0, -5.0, true));
        dots.push_back(std::make_shared<Dot>(2.5, 5.0, false));
    }

}
